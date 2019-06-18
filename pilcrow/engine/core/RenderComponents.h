#pragma once
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "pilcrow/modules/jellyfish_renderer/Jellyfish.h"

// base
struct Renderable {};

struct RenderText : Renderable {
  std::string Text;
  glm::vec2   Position{0.f, 0.f};
  glm::vec3   Color{.5f, .8f, .2f};
  float       Size{1.f};
};

struct CModel : Renderable {
  // TESTING: use Jellyfish::Model

  CModel(const std::string &file) : model(new Jellyfish::Model(file)) {}
  CModel(const CModel &) = default;
  CModel(CModel &&)      = default;
  CModel &operator=(const CModel &) = default;

  std::shared_ptr<Jellyfish::Model> model;
};