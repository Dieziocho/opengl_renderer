#pragma once
#include "shader_class.h"

namespace Shaders {
  struct ShaderGroup {
    Shader* solid;
    Shader* transparent;
  };

  extern ShaderGroup generic_2d;
  extern ShaderGroup generic_3d;
  extern ShaderGroup animated_3d;
  extern ShaderGroup lines;
  extern Shader composite;

  void compile();
}
