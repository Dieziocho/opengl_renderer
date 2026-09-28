#pragma once
#include "animator.h"
#include "model.h"
#include "ssbo_class.h"

class ModelInstance {
public:
  ModelInstance() = default;
  ModelInstance(Model& model) : model(&model), animator(model){
    copyModelBoneStates();
    size_t bone_count = model.bones.size();
    bones_transforms.reserve<glm::mat4>(bone_count);
    updateBones();
  }

  void playAnimation(const char* name){
    animator.play(name, true);
  }

  void update(){
    animator.update(bone_states);
    updateBones();
  }

  BoneState& getBone(unsigned id){
    return bone_states[id];
  }

  const glm::mat4& getTransform() const {
    return transform;
  }

  const SSBO& getBonesTransforms() const {
    return bones_transforms;
  }

private:
  const Model* model;
  glm::mat4 transform = 1;
  std::vector<BoneState> bone_states;
  SSBO bones_transforms;
  Animator animator;

  void updateBones(){
    std::vector<glm::mat4> result(model->bones.size());
    updateBones(result, model->root_id);
    bones_transforms.subData(result);
    bones_transforms.bind();
    bones_transforms.bindBase(0);
  }

  void updateBones(std::vector<glm::mat4>& output, unsigned id, const glm::mat4& parent = 1){
    glm::mat4 transform = parent * bone_states[id].getTransform();
    output[id] = transform * model->bones[id].offset;

    for(auto child : model->bones[id].children)
      updateBones(output, child, transform);
  }

  void copyModelBoneStates(){
    bone_states.reserve(model->bones.size());
    for(auto& bone : model->bones)
      bone_states.emplace_back(bone.default_state);
  }
};
