#include "animator.h"
#include "model.h"
#include <GLFW/glfw3.h>

Animator::Animator(const Model& model) : model(&model){
  reset();
}

void Animator::play(const char* animation_name, bool loop){
  loop_animation = loop;
  current_animation = &model->animations[animation_name];
  reset();
}

void Animator::reset(){
  base_time = getTime();
}

void Animator::update(std::vector<BoneState>& bones){
  if(!current_animation) return;

  float delta = getTime() - base_time;
  if(delta >= current_animation->duration && !loop_animation) return;

  for(auto& [bone_name, bone_id] : model->bones.map()){
    auto result = current_animation->getTransform(bone_name, delta);
    if(result.has_value()){
      BoneState& bone = bones[bone_id];
      Keyframe keyframe = result.value();
      bone.position = keyframe.position;
      bone.rotation = keyframe.rotation;
      bone.scale = keyframe.scale;
    }
  }
}

float Animator::getTime(){
  return glfwGetTime();
}
