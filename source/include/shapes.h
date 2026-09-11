#pragma once
#include "vertex.h"
#include <vector>

namespace Square {
  inline std::vector<Vertex2> vertices({
    Vertex2({0.0f, 0.0f}, {0.0f, 0.0f}),
    Vertex2({1.0f, 0.0f}, {1.0f, 0.0f}),
    Vertex2({1.0f, 1.0f}, {1.0f, 1.0f}),
    Vertex2({0.0f, 1.0f}, {0.0f, 1.0f}),
  });

  inline std::vector<unsigned> indices({
    0, 1, 2, 0, 2, 3
  });
}

