#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform 
{
	glm::vec3 position{ 0.f, 0.f, 0.f };
  glm::quat rotation;
	glm::vec3 scale{ 1.f, 1.f, 1.f };
};