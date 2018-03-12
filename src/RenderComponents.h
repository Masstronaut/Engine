#pragma once
#include <string>
#include <glm/glm.hpp>
struct RenderText {
  std::string Text;
  glm::vec2 Position{ 0.f, 0.f };
  glm::vec3 Color{ .5f,.8f,.2f };
  float Size{ 1.f };
};

