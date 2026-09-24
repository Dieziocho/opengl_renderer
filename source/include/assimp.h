#pragma once
#include "assimp/Importer.hpp"
#include "model.h"
#include <filesystem>

namespace Assimp {
  using Path = std::filesystem::path;
  extern thread_local Assimp::Importer importer;

  Model loadModel(const Path& file_path, unsigned flags = 0);

  glm::vec3 convertVec3(const aiVector3D& vector);
  glm::quat convertQuat(const aiQuaternion& quaternion);
  glm::mat4 convertMat4(const aiMatrix4x4& matrix);
  glm::mat4 operator*(const glm::mat4& left, const aiMatrix4x4& right);
}
