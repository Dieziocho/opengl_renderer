#pragma once
#include "model.h"
#include "animation.h"
#include <GLFW/glfw3.h>

class Animator {
public:
  Animator(Model& model, const Animation& animation) : model(model), animation(&animation){
    reset();
  }

  void reset(){
    base_time = glfwGetTime();
  }

  void update(){
    float delta = glfwGetTime() - base_time;

    for(auto& [bone_name, bone_id] : model.bones.map()){
      auto result = animation->getTransform(bone_name, delta);
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
  const Animation* animation;

  float base_time = 0;
};
