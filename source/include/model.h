#pragma once
#include "mesh.h"
#include "bone_list.h"
#include "animation_list.h"
#include "shaders.h"
#define MODEL_HAS_ANIMATIONS (1 << 0)

struct MeshInstance {
  unsigned index;
  glm::mat4 transformation;
};

struct Model {
  std::vector<Mesh> meshes;
  std::vector<MeshInstance> mesh_instances;
  Shaders::ShaderGroup* shaders;
  BoneList bones;
  AnimationList animations;
  unsigned root_id;

  unsigned flags = 0;
};
