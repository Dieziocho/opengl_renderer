#include "animator.h"
#include "render.h"
#include "camera.h"
#include "input.h"
#include "draw.h"
#include "assimp.h"
#include "shaders.h"

void Render::mainLoop(GLFWwindow* window){
  Model helios_model = Assimp::loadModel("resources/Helios/helios_casual_pose.fbx");
  Model vampire_model = Assimp::loadModel("resources/Vampire/vampire.dae");
  Animation helios_animation = Assimp::loadAnimation("resources/Helios/helios_casual_pose.fbx");
  Animation vampire_animation = Assimp::loadAnimation("resources/Vampire/vampire.dae");

  Animator helios_animator(helios_model, helios_animation);
  Animator vampire_animator(vampire_model, vampire_animation);
  while(!glfwWindowShouldClose(window)){
    Camera::update();

    helios_animator.update();
    vampire_animator.update();

    drawModel(helios_model, Shaders::animated_3d, {},
              {{helios_model.bones_transforms, 0}}
              );

    drawModel(vampire_model, Shaders::animated_3d, {},
              {{vampire_model.bones_transforms, 0}}
              );

    //Finish loop
    render(window);
    handleInput(window);
  }
}
