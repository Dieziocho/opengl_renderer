#pragma once
#include "glm/ext/matrix_transform.hpp"

inline constexpr glm::mat4 translate(float x, float y, float z){
  return glm::translate(glm::mat4(1), {x,y,z});
}

inline constexpr glm::mat4 scale(float scale){
  return glm::scale(glm::mat4(1), {scale, scale, scale});
}

inline constexpr glm::mat4 scale(float x, float y, float z){
  return glm::scale(glm::mat4(1), {x,y,z});
}

inline constexpr glm::mat4 translate(const glm::vec3& vec){
  return glm::translate(glm::mat4(1), vec);
}
