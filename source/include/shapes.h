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

namespace Cube {
  inline std::vector<glm::vec3> positions({
    glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(+1.0f, -1.0f, -1.0f),
    glm::vec3(+1.0f, +1.0f, -1.0f),
    glm::vec3(-1.0f, +1.0f, -1.0f),
    glm::vec3(-1.0f, -1.0f, +1.0f),
    glm::vec3(+1.0f, -1.0f, +1.0f),
    glm::vec3(+1.0f, +1.0f, +1.0f),
    glm::vec3(-1.0f, +1.0f, +1.0f),
  });

  inline std::vector<unsigned> indices({
    0, 1, 2, 2, 3, 0,
    4, 6, 5, 6, 4, 7,
    4, 0, 3, 3, 7, 4,
    1, 5, 6, 6, 2, 1,
    4, 5, 1, 1, 0, 4,
    3, 2, 6, 6, 7, 3,
  });
}

namespace Lines {
  inline Mesh makeMesh(std::vector<glm::vec3> vertices, std::vector<unsigned> indices){
    DataBuffer buffer;
    buffer.attach(0, vertices);
    buffer.setIndices(indices);

    return Mesh::FromBuffer(std::move(buffer), nullptr, MESH_LINES);
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
