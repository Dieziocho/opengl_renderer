#include "window.h"
#include <iostream>

namespace Window {
  void resizeCallback(GLFWwindow*, int width, int height){
    std::cout << "Resizing window to " << width << 'x' << height << std::endl;
    glViewport(0, 0, width, height);
  }
}

