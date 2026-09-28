#pragma once
#include "animation.h"
#include "bone.h"

class Model;
class ModelInstance;

class Animator {
public:
  Animator() = default;
  Animator(const Model& model);
  void play(const char* animation_name, bool loop = false);
  void reset();
  void update(std::vector<BoneState>& bones);

private:
  const Model* model;

  float base_time = 0;
  bool loop_animation = false;
  const Animation* current_animation = nullptr;

  float getTime();
};
