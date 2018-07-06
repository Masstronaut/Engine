#pragma once
#include "Camera.hpp"
#include "RenderComponents.h"
#include "components/Transform.h"
#include "entity/EntitiesWith.hpp"

#include "SettingsFileReader.hpp"
#include <Jellyfish.h>

struct WindowManager {
  WindowManager();

  void Init(World &world);

  EntitiesWith<Camera> Entities;
  void                 FrameStart();
  void                 FrameEnd();
  float                Dt{0.f};

private:
  // render context
  Jellyfish::iWindow *pWindow{nullptr};

  void      ProcessInput(Camera &cam);
  glm::vec2 m_windowSizeSetting{g_InitialWindowWidth, g_InitialWindowHeight};
  bool      m_windowFullscreenSetting{g_StartFullscreen};
};

struct RenderSystem {
  void Init(World &world) {
    // Register for Type Lists
    world.RegisterEntitiesWith(camEntities);
    world.RegisterEntitiesWith(textEntities);

    world.On([&](const Jellyfish::GLWindow::EWindowResized &event) {
      m_windowSize = event.newSize;
    });

    program.Load();

    // RENDERER LIB TEST
    Jellyfish::Derp test;
    test.DoAThing();  // prints a thing
  }

  void PreProcess() {
    // set up projetion matrices
    m_ortho_projection = glm::ortho(0.f, m_windowSize.x, 0.f, m_windowSize.y);
    if(camEntities.cbegin() != camEntities.cend()) {
      camera = &camEntities[0].Get<const Camera>();
      m_persp_projection
        = glm::perspective(glm::radians(camera->fov),
                           m_windowSize.x / m_windowSize.y, camera->nearplane,
                           camera->farplane);
      program.SetUniform("projection", m_persp_projection);
      program.SetUniform("view", camera->View());
    }
  }

  void Process(const CModel &model, const Transform &tf) const {
    if(!camera) return;
    glm::mat4 modelMatrix;
    modelMatrix = glm::translate(modelMatrix, tf.position);
    modelMatrix = glm::scale(modelMatrix, tf.scale);

    modelMatrix
      = glm::rotate(modelMatrix, tf.rotation.x, glm::vec3(1.f, 0.f, 0.f));
    modelMatrix
      = glm::rotate(modelMatrix, tf.rotation.y, glm::vec3(0.f, 1.f, 0.f));
    modelMatrix
      = glm::rotate(modelMatrix, tf.rotation.z, glm::vec3(0.f, 0.f, 1.f));

    program.SetUniform("model", modelMatrix);

    model.model->AssignShaderToAllMeshes(program);
    model.model->Draw();  // program no longer neede as arg textures TODO
  }

  float NextTextPos(float prevPos) {
    int offset = DEBUG_TEXT_SIZE;
    return prevPos - offset;
  }

  void PostProcess() {
    // Render Text
    for(const auto &entity : textEntities) {
      const RenderText &renderable{entity.Get<const RenderText>()};
      gltr.Render(renderable.Text, renderable.Position, m_ortho_projection,
                  renderable.Color, renderable.Size);
    }

    float position;
    gltr.Render("FPS: " + std::to_string(1.f / Dt),
                {0.f, position = NextTextPos(m_windowSize.y)},
                m_ortho_projection, {.5f, .8f, .2f});
    gltr.Render("Camera Pos X: " + std::to_string(camera->position.x),
                {0.f, position = NextTextPos(position)}, m_ortho_projection,
                {0.f, 0.f, 1.f});
    gltr.Render("Camera Pos Y: " + std::to_string(camera->position.y),
                {0.f, position = NextTextPos(position)}, m_ortho_projection,
                {0.f, 0.f, 1.f});
    gltr.Render("Camera Pos Z: " + std::to_string(camera->position.z),
                {0.f, position = NextTextPos(position)}, m_ortho_projection,
                {0.f, 0.f, 1.f});
  }

  void Draw() {
    // TODO
  }

  void FrameEnd() {
    // send to Renderer
    Draw();
  }

  // Type Lists
  EntitiesWith<const Camera>     camEntities;
  EntitiesWith<const RenderText> textEntities;

  // GL Impl
  Jellyfish::GLText            gltr{"Text.sprog"};
  mutable Jellyfish::GLProgram program{"Model.sprog"};

  float         Dt{0.f};
  glm::mat4     m_persp_projection;
  glm::mat4     m_ortho_projection;
  const Camera *camera{nullptr};

  // TODO: Use events instead
  glm::vec2 m_windowSize{g_InitialWindowWidth, g_InitialWindowHeight};
};
