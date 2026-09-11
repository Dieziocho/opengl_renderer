#include "opengl.h"
#include "shaders.h"
#include "camera.h"
#include "render.h"
#include <cstdio>

int main(){
  printf("Hello, world!\n");

  auto* window = OpenGL::init();

  Shaders::compile();
  Camera::init();
  Render::setup();
  Render::mainLoop(window);

  OpenGL::exit();
  return 0;
}
