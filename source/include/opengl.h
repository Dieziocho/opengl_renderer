#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace OpenGL {
  GLFWwindow* init();
  void exit();
}

constexpr float zero[] = {0, 0, 0, 0};
constexpr float one[] = {1, 1, 1, 1};
