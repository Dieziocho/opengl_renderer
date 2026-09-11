#include "shaders.h"

namespace Shaders {
  ShaderGroup generic_2d;

  Shader solid_2d;
  Shader transparent_2d;
  Shader composite;

  void compile(){
    generic_2d = ShaderGroup{
      .solid = &solid_2d,
      .transparent = &transparent_2d
    };

    solid_2d = Shader("source/shaders/generic_2d.vert", "source/shaders/solid.frag");
    transparent_2d = Shader("source/shaders/generic_2d.vert", "source/shaders/transparent.frag");
    composite = Shader("source/shaders/generic_2d.vert", "source/shaders/composite.frag");

    composite.setUniform("accumulate", 1);
    composite.setUniform("reveal", 2);
  }
}
