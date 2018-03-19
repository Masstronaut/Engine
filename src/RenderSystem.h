#pragma once
#include "Window.hpp"
#include "GLFWWindow.hpp"
#include "Camera.hpp"
#include "Entity/EntitiesWith.hpp"
#include "RenderComponents.h"
#include "Renderer.hpp"
#include "GLProgram.hpp"
#include "Model.hpp"
#include "Components/Transform.h"
#include "Camera.hpp"

struct WindowManager {
  WindowManager();

  void Init(World& world)
  {
	  window.On<GLWindow::EWindowResized>([&](const GLWindow::EWindowResized &event) {
		  world.Emit(event);
	  });
  }

  EntitiesWith<Camera> Entities;
  void FrameStart( );
  void FrameEnd( );
  float Dt{ 0.f };

private:
	//render context
  GLFWWindow window;
  void ProcessInput( Camera &cam );
};

struct RenderSystem {
	
	void Init(World& world)
	{
		//Register for Type Lists
		world.RegisterEntitiesWith(camEntities);
		world.RegisterEntitiesWith(textEntities);

		world.On<GLWindow::EWindowResized>([&](const GLWindow::EWindowResized &event) {
			m_windowSize = event.newSize;
		});

		program.Load();
	}

	void PreProcess() {
		
		//set up projetion matrices
		m_ortho_projection = glm::ortho(0.f, m_windowSize.x, 0.f, m_windowSize.y);
		if (camEntities.cbegin() != camEntities.cend()) {
			camera = &camEntities[0].Get<const Camera>();
			m_persp_projection = glm::perspective(glm::radians(camera->fov), m_windowSize.x / m_windowSize.y, camera->nearplane, camera->farplane);
			program.SetUniform("projection", m_persp_projection);
			program.SetUniform("view", camera->View());
		}
	}

	void Process(const CModel &model, const Transform &tf) const {
		
		if (!camera) return;
		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(modelMatrix, tf.pos);
		modelMatrix = glm::scale(modelMatrix, tf.scale);
		program.SetUniform("model", modelMatrix);
		model.model->Draw(program);
	}

	void PostProcess() {

		//Render Text
		for (const auto &entity : textEntities) {
			const RenderText &renderable{ entity.Get<const RenderText>() };
			gltr.Render(renderable.Text, renderable.Position, m_ortho_projection, renderable.Color, renderable.Size);
		}

		gltr.Render("FPS: " + std::to_string(1.f / Dt), { 0.f,0.f }, m_ortho_projection, { .5f,.8f,.2f });
	}

	void Draw() {
		//TODO
	}

	void FrameEnd() {
		//send to Renderer
		Draw();
	}


	//Type Lists
	EntitiesWith<const Camera> camEntities;
	EntitiesWith<const RenderText> textEntities;

	//GL Impl 
	GLTextRenderer gltr{ "Text.sprog" };
	mutable GLProgram program{ "Model.sprog" };

	float Dt{ 0.f };
	glm::mat4 m_persp_projection;
	glm::mat4 m_ortho_projection;
	const Camera *camera{ nullptr };
	glm::vec2 m_windowSize{g_InitialWindowWidth, g_InitialWindowHeight };
};