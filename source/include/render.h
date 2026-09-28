#pragma once
#include "mesh.h"
#include "shaders.h"

namespace Render {
  void setup();
  void mainLoop(GLFWwindow* window);

  void addDrawCall(const Mesh& mesh, Shaders::ShaderGroup& shaders, const glm::mat4 transform,
                   const std::vector<UniformCall>& uniforms, const std::vector<SSBOCall>& ssbos);
  void render(GLFWwindow* window);
}
