#pragma once
#include "shader_class.h"

namespace Shaders {
  struct ShaderGroup {
    Shader* solid;
    Shader* transparent;
  };

  extern ShaderGroup generic_2d;
  extern Shader composite;

  void compile();
}
