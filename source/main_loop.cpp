#include "render.h"
#include "mesh.h"
#include "draw.h"
#include "glm.h"
#include "camera.h"
#include "input.h"
#include "shapes.h"

void Render::mainLoop(GLFWwindow* window){
  Mesh red = Mesh::FromTexture(Texture::Color(0x800000ff));
  Mesh green = Mesh::FromTexture(Texture::Color(0x8000ff00));
  Mesh blue = Mesh::FromTexture(Texture::Color(0x80ff0000));

  red.addFlags(MESH_TRANSPARENT);
  green.addFlags(MESH_TRANSPARENT);
  blue.addFlags(MESH_TRANSPARENT);

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

    drawMesh(red,   Shaders::generic_2d, translate(0.5,0,0));
    drawMesh(green, Shaders::generic_2d, translate(0,0,1));
    drawMesh(blue,  Shaders::generic_2d, translate(0,0,-1));
    drawMesh(lines, Shaders::lines, translate(0,0,4));

    //Finish loop
    render(window);
    handleInput(window);
  }
}
