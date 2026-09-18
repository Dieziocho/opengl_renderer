#pragma once
#include "glm/ext/vector_int4.hpp"
#define BONE_COUNT glm::ivec4::length()

struct BoneData {
  //Bone indexes which will influence this vertex
  int bone_ids[BONE_COUNT] = {-1, -1, -1, -1};
  //Weights from each bone
  float weights[BONE_COUNT] = {0, 0, 0, 0};
};
