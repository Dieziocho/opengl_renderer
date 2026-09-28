#include "render.h"
#include "camera.h"
#include "input.h"

void setups();
void onFrame();

void Render::mainLoop(GLFWwindow* window){
  setups();

  while(!glfwWindowShouldClose(window)){
    Camera::update();

    onFrame();

    //Finish loop
    render(window);
    handleInput(window);
  }
}
