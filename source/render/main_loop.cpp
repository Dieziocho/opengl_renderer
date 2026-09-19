#include "render.h"
#include "camera.h"
#include "input.h"
#include "draw.h"
#include "assimp.h"
#include "shaders.h"
#include "shapes.h"
#include "glm.h"
#include <GLFW/glfw3.h>

void Render::mainLoop(GLFWwindow* window){
  Model model = Assimp::loadModel("resources/Helios/helios.fbx");
  Mesh lines = Lines::makeMesh(Cube::positions);

  while(!glfwWindowShouldClose(window)){
    Camera::update();

    model.bones["Root"].rotation = glm::angleAxis((float)glfwGetTime(), glm::vec3(0,1,0));
    model.bones["LowerArm.L.001"].rotation = glm::angleAxis((float)glfwGetTime(), glm::vec3(0,0,1));
    model.updateBones();

    drawModel(model, Shaders::animated_3d);
    drawMesh(lines, Shaders::lines, translate(5,0,0));

    //Finish loop
    render(window);
    handleInput(window);
  }
}
