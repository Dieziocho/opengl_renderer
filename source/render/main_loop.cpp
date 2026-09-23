#include "animator.h"
#include "render.h"
#include "camera.h"
#include "input.h"
#include "draw.h"
#include "assimp.h"
#include "shaders.h"

void Render::mainLoop(GLFWwindow* window){
  Model helios_model = Assimp::loadModel("resources/Helios/helios.fbx");

  while(!glfwWindowShouldClose(window)){
    Camera::update();

    drawModel(helios_model, Shaders::animated_3d, {},
              {{helios_model.bones_transforms, 0}}
              );

    //Finish loop
    render(window);
    handleInput(window);
  }
}
