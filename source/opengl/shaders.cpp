#include "shaders.h"

namespace Shaders {
  ShaderGroup generic_2d;
  ShaderGroup generic_3d;
  ShaderGroup lines;
  Shader composite;

  Shader solid_2d;
  Shader solid_3d;
  Shader transparent_2d;
  Shader transparent_3d;
  Shader lines_3d;

  void compile(){
    generic_2d = ShaderGroup{
      .solid = &solid_2d,
      .transparent = &transparent_2d
    };
    generic_3d = ShaderGroup{
      .solid = &solid_3d,
      .transparent = &transparent_3d
    };
    lines = ShaderGroup{
      .solid = &lines_3d,
      .transparent = &lines_3d
    };

    solid_2d = Shader("source/shaders/generic_2d.vert", "source/shaders/solid.frag");
    solid_3d = Shader("source/shaders/generic_3d.vert", "source/shaders/solid.frag");
    transparent_2d = Shader("source/shaders/generic_2d.vert", "source/shaders/transparent.frag");
    transparent_3d = Shader("source/shaders/generic_3d.vert", "source/shaders/transparent.frag");
    lines_3d = Shader("source/shaders/lines.vert", "source/shaders/lines.frag");

    composite = Shader("source/shaders/stub_2d.vert", "source/shaders/composite.frag");
    composite.setUniform("accumulate", 1);
    composite.setUniform("reveal", 2);
  }
}
