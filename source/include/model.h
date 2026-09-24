#pragma once
#include "mesh.h"
#include "bone_list.h"
#include "animation_list.h"

struct MeshInstance {
  unsigned index;
  glm::mat4 transformation;
};

struct Model {
  std::vector<Mesh> meshes;
  std::vector<MeshInstance> mesh_instances;
  BoneList bones;
  AnimationList animations;
  unsigned root_id;

  unsigned model_id = 0;
  inline static unsigned current_model_id = 1;
};
