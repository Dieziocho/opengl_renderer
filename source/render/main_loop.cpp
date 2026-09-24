#include "render.h"
#include "camera.h"
#include "input.h"
#include "draw.h"
#include "assimp.h"
#include "shaders.h"
#include "model_instance.h"
#include <GLFW/glfw3.h>

void Render::mainLoop(GLFWwindow* window){
  Model helios_model = Assimp::loadModel("resources/Helios/test.fbx");
  ModelInstance helios(helios_model);
  helios.playAnimation("Hi");

  while(!glfwWindowShouldClose(window)){
    Camera::update();

    helios.update();
    drawModel(helios_model, Shaders::animated_3d, helios.getTransform(), {},
              {{helios.getBonesTransforms(), 0}}
              );

    //Finish loop
    render(window);
    handleInput(window);
  }
}
