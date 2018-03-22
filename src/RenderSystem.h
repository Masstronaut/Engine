#pragma once
#include "Window.hpp"
#include "GLFWWindow.hpp"
#include "Camera.hpp"
#include "Entity/EntitiesWith.hpp"
#include "RenderComponents.h"
#include "GLTextRenderer.hpp"
#include "GLProgram.hpp"
#include "Model.hpp"
#include "Components/Transform.h"
#include "Camera.hpp"
#include <Jellyfish.h>


struct WindowManager {
  WindowManager();

  void Init(World& world)
  {
	  window.On<GLWindow::EWindowResized>([&](const GLWindow::EWindowResized &event) {
		  world.Emit(event);
	  });
    window.On<GLWindow::EWindowStateChanged>([&](const GLWindow::EWindowStateChanged &event) {
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


		//RENDERER LIB TEST
		Jellyfish::Derp test;
		test.DoAThing();

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

		modelMatrix = glm::rotate(modelMatrix, tf.rot.x, glm::vec3(1.f, 0.f, 0.f));
		modelMatrix = glm::rotate(modelMatrix, tf.rot.y, glm::vec3(0.f, 1.f, 0.f));
		modelMatrix = glm::rotate(modelMatrix, tf.rot.z, glm::vec3(0.f, 0.f, 1.f));
		
		program.SetUniform("model", modelMatrix);
		model.model->Draw(program);
	}


	float NextTextPos(float prevPos)
	{
		int offset = g_DebugTextSize;
		return prevPos - offset;
	}

	void PostProcess() {

		//Render Text
		for (const auto &entity : textEntities) {
			const RenderText &renderable{ entity.Get<const RenderText>() };
			gltr.Render(renderable.Text, renderable.Position, m_ortho_projection, renderable.Color, renderable.Size);
		}

		float pos;
		gltr.Render("FPS: " + std::to_string(1.f / Dt), { 0.f, pos=NextTextPos(m_windowSize.y) }, m_ortho_projection, { .5f,.8f,.2f });
		gltr.Render("Camera Pos X: " + std::to_string(camera->position.x), { 0.f, pos = NextTextPos(pos) }, m_ortho_projection, { 0.f, 0.f, 1.f });
		gltr.Render("Camera Pos Y: " + std::to_string(camera->position.y), { 0.f, pos = NextTextPos(pos) }, m_ortho_projection, { 0.f, 0.f, 1.f });
		gltr.Render("Camera Pos Z: " + std::to_string(camera->position.z), { 0.f, pos = NextTextPos(pos) }, m_ortho_projection, { 0.f, 0.f, 1.f });
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