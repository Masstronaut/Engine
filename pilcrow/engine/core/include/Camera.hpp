#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
  glm::vec3 position{0.f, 2.f, 0.f};
  glm::vec3 up{0.f, 1.f, 0.f};
  float     yaw{0.f};
  float     pitch{0.f};
  float     fov{80.f};
  float     nearplane{0.1f};
  float     farplane{100.f};
  glm::mat4 LookAt(glm::vec3 target) {
    glm::vec3 right{Right()};
    return glm::mat4{right.x,    right.y,    right.z,    0,
                     up.x,       up.y,       up.z,       0,
                     target.x,   target.y,   target.z,   0,
                     position.x, position.y, position.z, 1.f};
  }
  glm::vec3 Front() const {
    return glm::normalize(
      glm::vec3{cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
                sin(glm::radians(pitch)),
                cos(glm::radians(pitch)) * sin(glm::radians(yaw))});
  }
  glm::vec3 Direction() const { return glm::normalize(position - Front()); }
  glm::vec3 Right() const { return glm::normalize(glm::cross(Front(), up)); }
  glm::mat4 View() const {
    return glm::lookAt(position, position + Front(), up);
  }
};