#include "animator.h"
#include "render.h"
#include "camera.h"
#include "input.h"
#include "draw.h"
#include "assimp.h"
#include "shaders.h"
#include <GLFW/glfw3.h>

void Render::mainLoop(GLFWwindow* window){
  Model helios_model = Assimp::loadModel("resources/Helios/helios.fbx");
  Animation helios_animation = Assimp::loadAnimation("resources/Helios/helios.fbx");

  Animator helios_animator(helios_model, helios_animation);
  while(!glfwWindowShouldClose(window)){
    Camera::update();

    helios_animator.update();
    drawModel(helios_model, Shaders::animated_3d);

    //Finish loop
    render(window);
    handleInput(window);
  }
}
