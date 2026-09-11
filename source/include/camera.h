#pragma once
#include "glm/ext/matrix_float4x4.hpp"

namespace Camera {
  void init();
  void update();

  void moveBy(glm::vec3 offset);
  void rotateBy(glm::vec2 offset);

  std::array<glm::vec3, 3> getDirections();
  glm::vec3 getPosition();
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();
}
