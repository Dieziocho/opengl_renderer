#pragma once
#include "glm/ext/quaternion_float.hpp"
#include <vector>

struct KeyRotation {
  glm::quat rotation;
  float time;
};
struct KeyScale {
  glm::quat scale;
  float time;
};

struct Animation {
  std::vector<KeyRotation> rotation;
  std::vector<KeyScale> scale;
};
