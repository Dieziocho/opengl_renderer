#include "render.h"
#include "camera.h"
#include "input.h"
#include "draw.h"
#include "assimp.h"
#include "shaders.h"

void Render::mainLoop(GLFWwindow* window){
  Model model = Assimp::loadModel("resources/Helios/helios.fbx");
  Mesh red = Mesh::FromColor(0xff0000ff);

  while(!glfwWindowShouldClose(window)){
    Camera::update();

    drawModel(model, Shaders::generic_3d);

    //Finish loop
    render(window);
    handleInput(window);
  }
}
