#include "input.h"
#include "camera.h"
#include "config.h"

void handleInput(GLFWwindow* window){
  glfwPollEvents();
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, true);
  }

  glm::vec3 offset = {0,0,0};
  auto [front, right, up] = Camera::getDirections();

  if(glfwGetKey(window, GLFW_KEY_W)) offset += front * +player_speed;
  if(glfwGetKey(window, GLFW_KEY_S)) offset += front * -player_speed;
  if(glfwGetKey(window, GLFW_KEY_A)) offset += right * -player_speed;
  if(glfwGetKey(window, GLFW_KEY_D)) offset += right * +player_speed;

  if(glfwGetKey(window, GLFW_KEY_SPACE)) offset += world_up * +player_speed;
  if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) offset += world_up * -player_speed;

  if(offset != glm::vec3(0,0,0))
    Camera::moveBy(offset);
}


