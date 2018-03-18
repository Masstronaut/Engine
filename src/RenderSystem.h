#pragma once
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
	
	//register for type lists
	EntitiesWith<const Camera> camEntities;
	EntitiesWith<const RenderText> textEntities;

	RenderSystem()
	{
		program.Load();
	}
	
	void Init(World& world)
	{
		world.RegisterEntitiesWith(camEntities);
		world.RegisterEntitiesWith(textEntities);
	}

	void PreProcess() {
		if (camEntities.cbegin() != camEntities.cend()) {
			camera = &camEntities[0].Get<const Camera>();
			projection = glm::perspective(glm::radians(camera->fov), 800.f / 600.f, camera->nearplane, camera->farplane);
			program.SetUniform("projection", projection);
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
		for (const auto &entity : textEntities) {
			const RenderText &renderable{ entity.Get<const RenderText>() };
			gltr.Render(renderable.Text, renderable.Position, renderable.Color, renderable.Size);
		}
	}

	void Draw() {
		//TODO
	}

	void FrameEnd() {
		//send to Renderer
		Draw();
	}

	GLTextRenderer gltr{ "Text.sprog" };
	mutable GLProgram program{ "Model.sprog" };

	glm::mat4 projection;
	const Camera *camera{ nullptr };
};


struct FrameratePrinter {
  void PostProcess( ) {
    gltr.Render( "FPS: " + std::to_string( 1.f / Dt ), { 0.f,0.f }, { .5f,.8f,.2f } );
  }
  float Dt{ 0.f };
  GLTextRenderer gltr{ "Text.sprog" };
};