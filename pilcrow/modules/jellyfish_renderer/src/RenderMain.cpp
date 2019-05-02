#pragma once

//std
#include <iostream>

//ours
#include "../Include/Jellyfish.h"

namespace Jellyfish
{
	void Derp::DoAThing()
	{
		std::cout << str << std::endl;
	}

	

	//void Init(World& world)
	//{
	//	//Register for Type Lists
	//	world.RegisterEntitiesWith(camEntities);
	//	world.RegisterEntitiesWith(textEntities);
	//
	//	world.On([&](const Jellyfish::GLWindow::EWindowResized &event) {
	//		m_windowSize = event.newSize;
	//	});
	//
	//	//Load shader -- probably should move this
	//	program.Load();
	//}
	//
	//void PreProcess()
	//{
	//	//set up projetion matrices
	//	m_ortho_projection = glm::ortho(0.f, m_windowSize.x, 0.f, m_windowSize.y);
	//	if (camEntities.cbegin() != camEntities.cend()) {
	//		camera = &camEntities[0].Get<const Camera>();
	//		m_persp_projection = glm::perspective(glm::radians(camera->fov), m_windowSize.x / m_windowSize.y, camera->nearplane, camera->farplane);
	//		program.SetUniform("projection", m_persp_projection);
	//		program.SetUniform("view", camera->View());
	//	}
	//}
	//
	//void Process(const CModel &model, const Transform &tf) const
	//{
	//	if (!camera) return;
	//	glm::mat4 modelMatrix;
	//	modelMatrix = glm::translate(modelMatrix, tf.position);
	//	modelMatrix = glm::scale(modelMatrix, tf.scale);
	//
	//	modelMatrix = glm::rotate(modelMatrix, tf.rotation.x, glm::vec3(1.f, 0.f, 0.f));
	//	modelMatrix = glm::rotate(modelMatrix, tf.rotation.y, glm::vec3(0.f, 1.f, 0.f));
	//	modelMatrix = glm::rotate(modelMatrix, tf.rotation.z, glm::vec3(0.f, 0.f, 1.f));
	//
	//	program.SetUniform("model", modelMatrix);
	//
	//	model.model->AssignShaderToAllMeshes(program);
	//	model.model->Draw(); //program no longer neede as arg textures TODO
	//}
	//
	//
	//float NextTextPos(float prevPos)
	//{
	//	int offset = DEBUG_TEXT_SIZE;
	//	return prevPos - offset;
	//}
	//
	//void PostProcess()
	//{
	//	//Render Text
	//	for (const auto &entity : textEntities)
	//	{
	//		const RenderText &renderable{ entity.Get<const RenderText>() };
	//		gltr.Render(renderable.Text, renderable.Position, m_ortho_projection, renderable.Color, renderable.Size);
	//	}
	//
	//	float position;
	//	gltr.Render("FPS: " + std::to_string(1.f / Dt), { 0.f, position = NextTextPos(m_windowSize.y) }, m_ortho_projection, { .5f,.8f,.2f });
	//	gltr.Render("Camera Pos X: " + std::to_string(camera->position.x), { 0.f, position = NextTextPos(position) }, m_ortho_projection, { 0.f, 0.f, 1.f });
	//	gltr.Render("Camera Pos Y: " + std::to_string(camera->position.y), { 0.f, position = NextTextPos(position) }, m_ortho_projection, { 0.f, 0.f, 1.f });
	//	gltr.Render("Camera Pos Z: " + std::to_string(camera->position.z), { 0.f, position = NextTextPos(position) }, m_ortho_projection, { 0.f, 0.f, 1.f });
	//}
	//
	//void Draw()
	//{
	//	//TODO
	//}
	//
	//void FrameEnd()
	//{
	//	//send to Renderer
	//	Draw();
	//}
}
