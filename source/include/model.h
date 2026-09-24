#pragma once
#include "animation_list.h"
#include "mesh.h"
#include "ssbo_class.h"

struct MeshInstance {
  unsigned index;
  glm::mat4 transformation;
};

struct Model {
  std::vector<Mesh> meshes;
  std::vector<MeshInstance> mesh_instances;
  glm::mat4 transform;
  AnimationList animations;

  Bones bones;
  SSBO bones_transforms;

  unsigned root_id;

  unsigned model_id = 0;
  inline static unsigned current_model_id = 1;

  void updateBones(){
    std::vector<glm::mat4> result(bones.size());
    updateBones(result, root_id);
    bones_transforms.subData(result);
    bones_transforms.bind();
    bones_transforms.bindBase(0);
  }

  void updateBones(std::vector<glm::mat4>& output, unsigned id, const glm::mat4& parent = 1){
    glm::mat4 transform = parent * bones[id].getTransform();
    output[id] = transform * bones[id].offset;

    for(auto child : bones[id].children)
      updateBones(output, child, transform);
  }
};
