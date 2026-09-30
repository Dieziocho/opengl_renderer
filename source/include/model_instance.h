#pragma once
#include "animator.h"
#include "model.h"
#include "ssbo_class.h"

class ModelInstance {
public:
  ModelInstance() = default;
  ModelInstance(const Model& model) : model(&model), animator(model){
    if(model.flags & MODEL_HAS_ANIMATIONS){
      copyModelBoneStates();
      size_t bone_count = model.bones.size();
      bones_transforms.reserve<glm::mat4>(bone_count);
      updateBones();
    }
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

  const SSBO& getBonesTransforms() const {
    return bones_transforms;
  }

  const Model& getModel() const {
    return *model;
  }

private:
  const Model* model;
  std::vector<BoneState> bone_states;
  SSBO bones_transforms;
  Animator animator;

  void updateBones(){
    if(model->flags & MODEL_HAS_ANIMATIONS){
      std::vector<glm::mat4> result(model->bones.size());
      updateBones(result, model->root_id);
      bones_transforms.subData(result);
      bones_transforms.bind();
      bones_transforms.bindBase(0);
    }
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
