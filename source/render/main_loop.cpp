#include "render.h"
#include "camera.h"
#include "input.h"
#include "draw.h"
#include "assimp.h"
#include "shaders.h"
#include "animator.h"
#include <GLFW/glfw3.h>

void Render::mainLoop(GLFWwindow* window){
  Model helios_model = Assimp::loadModel("resources/Helios/test.fbx");
  Animator animator(helios_model);
  animator.play("Hi", true);

  while(!glfwWindowShouldClose(window)){
    Camera::update();

    animator.update();
    drawModel(helios_model, Shaders::animated_3d, {},
              {{helios_model.bones_transforms, 0}}
              );

    //Finish loop
    render(window);
    handleInput(window);
  }
}
