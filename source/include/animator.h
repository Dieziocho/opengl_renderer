#pragma once
#include "animation.h"

class Model;
class ModelInstance;

class Animator {
public:
  Animator(const Model& model, ModelInstance& instance);
  void play(const char* animation_name, bool loop = false);
  void reset();
  void update();

private:
  const Model& model;
  ModelInstance& instance;

  float base_time = 0;
  bool loop_animation = false;
  const Animation* current_animation = nullptr;

  float getTime();
};
