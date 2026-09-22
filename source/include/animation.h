#pragma once
#include "glm/common.hpp"
#include "glm/ext/quaternion_float.hpp"
#include <algorithm>
#include <string>
#include <map>

struct KeyPosition {
  glm::vec3 value;
  float time;
};

struct KeyRotation {
  glm::quat value;
  float time;
};

struct KeyScale {
  glm::vec3 value;
  float time;
};

struct Keyframe {
  glm::vec3 position;
  glm::quat rotation;
  glm::vec3 scale;
};

struct Keyframes {
  std::vector<KeyPosition> positions;
  std::vector<KeyRotation> rotations;
  std::vector<KeyScale> scales;
};

template<typename T>
bool compareTime(float left, const T& right){
  return left < right.time;
}

template<typename T, typename L>
L interpolate(const std::vector<T>& items, float time){
  if(items.empty()) return L();
  if(items.size() == 1) return items.front().value;

  auto right_it = std::upper_bound(items.begin(), items.end(), time, compareTime<T>);

  if(right_it == items.begin()) return items.front().value;
  if(right_it == items.end())   return items.back().value;

  const T& right = *right_it;
  const T& left  = *(right_it - 1);

  float span = right.time - left.time;
  if(span <= 0) return left.value;

  float mix = (time - left.time) / span;
  return glm::mix(left.value, right.value, mix);
}

struct Animation {
  std::map<std::string, Keyframes> bones;
  float ticks_per_second;
  float duration;

  Keyframe getTransform(const std::string& bone_name, float time) const {
    auto it = bones.find(bone_name);
    if(it == bones.end()) return {};

    time *= ticks_per_second;
    time = fmod(time, duration);

    const Keyframes& keyframes = it->second;

    glm::vec3 position = interpolate<KeyPosition, glm::vec3>(keyframes.positions, time);
    glm::quat rotation = interpolate<KeyRotation, glm::quat>(keyframes.rotations, time);
    glm::vec3 scale = interpolate<KeyScale, glm::vec3>(keyframes.scales, time);

    return {position, rotation, scale};
  }
};
