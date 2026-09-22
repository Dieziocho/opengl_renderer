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
      Keyframe keyframe = animation->getTransform(bone_name, delta);
      model.bones[bone_id].position = keyframe.position;
      model.bones[bone_id].rotation = keyframe.rotation;
      model.bones[bone_id].scale = keyframe.scale;
    }

    model.updateBones();
  }

private:
  Model& model;
  const Animation* animation;

  float base_time = 0;
};
