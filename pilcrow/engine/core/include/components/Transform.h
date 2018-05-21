#pragma once
#include <glm/glm.hpp>

struct Transform {
  glm::vec3 position{0.f, 0.f, 0.f};
  glm::vec3 rotation{0.f, 0.f, 0.f};
  glm::vec3 scale{1.f, 1.f, 1.f};
};