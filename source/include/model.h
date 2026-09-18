#pragma once
#include "mesh.h"
#include <map>

struct MeshInstance {
  unsigned index;
  glm::mat4 transformation;
};

struct Bone {
  unsigned parent = -1u;
  std::vector<unsigned> children;
  glm::mat4 offset;
};

struct Model {
  std::vector<Mesh> meshes;
  std::vector<MeshInstance> mesh_instances;
  glm::mat4 transform;

  std::map<std::string, unsigned> bone_map;
  std::vector<Bone> bones;

  unsigned model_id = 0;
  inline static unsigned current_model_id = 1;
};
