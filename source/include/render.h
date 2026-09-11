#pragma once
#include "mesh.h"
#include "shaders.h"

namespace Render {
  void setup();
  void mainLoop(GLFWwindow* window);

  void addDrawCall(const Mesh& mesh, Shaders::ShaderGroup& shaders, const glm::mat4 transform);
  void drawCall(Shader& shader, const Mesh& mesh, const glm::mat4& transform);
  void render(GLFWwindow* window);
}
