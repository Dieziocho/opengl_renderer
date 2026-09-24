#pragma once
#include "glm/gtc/quaternion.hpp"
#define BONE_COUNT glm::ivec4::length()

struct BoneData {
  //Bone indexes which will influence this vertex
  int bone_ids[BONE_COUNT] = {-1, -1, -1, -1};
  //Weights from each bone
  float weights[BONE_COUNT] = {0, 0, 0, 0};
};

struct BoneState {
  glm::vec3 position = glm::vec3(0,0,0);
  glm::quat rotation = glm::quat(1,0,0,0);
  glm::vec3 scale = glm::vec3(1,1,1);

  glm::mat4 getTransform() const {
    return glm::translate(glm::mat4(1), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1), scale);
  }
};

struct BoneInfo {
  unsigned parent = -1u;
  std::vector<unsigned> children;
  glm::mat4 offset;

  BoneState default_state;
};
