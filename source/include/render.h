#pragma once
#include "mesh.h"
#include "shaders.h"

namespace Render {
  void setup();
  void mainLoop(GLFWwindow* window);

  void addDrawCall(const Mesh& mesh, Shaders::ShaderGroup& shaders, const glm::mat4 transform,
                   std::vector<UniformCall> uniforms, std::vector<SSBOCall> ssbos);
  void render(GLFWwindow* window);
}
