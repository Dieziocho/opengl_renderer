#pragma once
#include "model.h"
#include <GLFW/glfw3.h>

class Animator {
public:
  Animator(Model& model) : model(model){
    reset();
  }

  void play(const char* animation_name, bool loop = false){
    loop_animation = loop;
    current_animation = &model.animations[animation_name];
    reset();
  }

  void reset(){
    base_time = getTime();
  }

  void update(){
    if(!current_animation) return;

    float delta = getTime() - base_time;
    if(delta >= current_animation->duration && !loop_animation) return;

    for(auto& [bone_name, bone_id] : model.bones.map()){
      auto result = current_animation->getTransform(bone_name, delta);
      if(result.has_value()){
        model.bones[bone_id].position = result.value().position;
        model.bones[bone_id].rotation = result.value().rotation;
        model.bones[bone_id].scale = result.value().scale;
      }
    }

    model.updateBones();
  }

private:
  Model& model;

  float base_time = 0;
  bool loop_animation = false;
  const Animation* current_animation = nullptr;

  float getTime(){
    return glfwGetTime();
  }
};
