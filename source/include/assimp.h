#pragma once
#include "model.h"
#include <filesystem>

namespace Assimp {
  using Path = std::filesystem::path;

  Model loadModel(const Path& file_path, unsigned flags = 0);
}
