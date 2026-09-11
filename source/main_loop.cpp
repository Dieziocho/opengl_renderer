#include "mesh.h"
#include "render.h"
#include "draw.h"
#include "camera.h"
#include "input.h"
#include "shapes.h"
#include "glm/ext/matrix_transform.hpp"

void Render::mainLoop(GLFWwindow* window){
  Mesh red = Mesh::FromTexture(Texture::Color(0xff0000ff));
  Mesh green = Mesh::FromTexture(Texture::Color(0xff00ff00));
  Mesh blue = Mesh::FromTexture(Texture::Color(0xffff0000));

  std::vector<glm::vec3> vertices({
    {0.0f, 1.5f, 1.0f},
    {1.0f, 0.0f, 0.0f},
    {2.0f, 1.0f, 0.0f},
    {5.0f, 1.0f, 0.0f},
    {-2.0f, -4.0f, 0.0f}
  });

  Mesh lines = Lines::makeMesh(vertices);

  while(!glfwWindowShouldClose(window)){
    Camera::update();

    drawMesh(red, Shaders::generic_2d, glm::translate(glm::mat4(1), {0.5,0,0}));
    drawMesh(green, Shaders::generic_2d, glm::translate(glm::mat4(1), {0,0,1}));
    drawMesh(blue, Shaders::generic_2d, glm::translate(glm::mat4(1), {0,0,-1}));
    drawMesh(lines, Shaders::lines, glm::translate(glm::mat4(1), {0,0,4}));

    //Finish loop
    render(window);
    handleInput(window);
  }
}
