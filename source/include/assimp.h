#pragma once
#include "assimp/Importer.hpp"
#include "model.h"
#include "animation.h"
#include <filesystem>

namespace Assimp {
  using Path = std::filesystem::path;
  extern thread_local Assimp::Importer importer;

  Model loadModel(const Path& file_path, unsigned flags = 0);
  Animation loadAnimation(const Path& file_path, unsigned flags = 0);
}
