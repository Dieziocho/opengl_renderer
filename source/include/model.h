#pragma once
#include "animation_list.h"
#include "mesh.h"

struct MeshInstance {
  unsigned index;
  glm::mat4 transformation;
};

struct Model {
  std::vector<Mesh> meshes;
  std::vector<MeshInstance> mesh_instances;
  AnimationList animations;

  Bones bones;

  unsigned root_id;

  unsigned model_id = 0;
  inline static unsigned current_model_id = 1;
};
