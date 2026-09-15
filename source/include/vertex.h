#pragma once
#include "glm/ext/vector_float2.hpp"
#include <cstddef>

template<size_t count>
struct VertexData {
  using vec = glm::vec<count, float>;
  glm::vec<count, float> position;
  glm::vec2 texture_coordinates;
};

using Vertex2 = VertexData<2>;
using Vertex3 = VertexData<3>;
