#pragma once
#include "mesh.h"

struct MeshInstance {
  unsigned index;
  glm::mat4 transformation;
};

struct Model {
  std::vector<Mesh> meshes;
  std::vector<MeshInstance> mesh_instances;
  glm::mat4 transform;
};
