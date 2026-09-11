#include "shaders.h"

namespace Shaders {
  ShaderGroup generic_2d;
  ShaderGroup lines;

  Shader composite;

  Shader solid_2d;
  Shader transparent_2d;
  Shader lines_3d;

  void compile(){
    generic_2d = ShaderGroup{
      .solid = &solid_2d,
      .transparent = &transparent_2d
    };
    lines = ShaderGroup{
      .solid = &lines_3d,
      .transparent = &lines_3d
    };

    lines_3d = Shader("source/shaders/lines.vert", "source/shaders/lines.frag");
    composite = Shader("source/shaders/generic_2d.vert", "source/shaders/composite.frag");

    solid_2d = Shader("source/shaders/generic_2d.vert", "source/shaders/solid.frag");
    transparent_2d = Shader("source/shaders/generic_2d.vert", "source/shaders/transparent.frag");

    composite.setUniform("accumulate", 1);
    composite.setUniform("reveal", 2);
  }
}
