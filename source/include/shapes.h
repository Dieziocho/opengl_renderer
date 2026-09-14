#pragma once
#include "mesh.h"
#include "vertex.h"
#include <vector>

namespace Square {
  inline std::vector<Vertex2> vertices({
    Vertex2({-1.0f, -1.0f}, {0.0f, 0.0f}),
    Vertex2({+1.0f, -1.0f}, {1.0f, 0.0f}),
    Vertex2({+1.0f, +1.0f}, {1.0f, 1.0f}),
    Vertex2({-1.0f, +1.0f}, {0.0f, 1.0f}),
  });

  inline std::vector<unsigned> indices({
    0, 1, 2, 0, 2, 3
  });
}

namespace Lines {
  inline Mesh makeMesh(std::vector<glm::vec3> vertices, std::vector<unsigned> indices){
    return Mesh::FromBuffer(DataBuffer(vertices, indices, GL_LINES));
  }

  inline Mesh makeMesh(std::vector<glm::vec3> vertices){
    std::vector<unsigned> indices;
    for(unsigned i = 0; i < (vertices.size() - 1); ++i){
      indices.push_back(i);
      indices.push_back(i + 1);
    }
    return makeMesh(vertices, indices);
  }
}
