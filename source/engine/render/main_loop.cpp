#include "draw.h"
#include "engine.h"
#include "render.h"
#include "camera.h"
#include "input.h"

void setups();
void onFrame();

void Render::mainLoop(GLFWwindow* window){
  setups();

  while(!glfwWindowShouldClose(window)){
    Camera::update();

    onFrame();

    for(auto& entity : Engine::getEntities()){
      auto& instance = entity.getInstance();
      auto& model = instance.getModel();
      instance.update();
      drawModel(model, *model.shaders, entity.getTransform(), {}, {{instance.getBonesTransforms(), 0}, {model.bones_offset, 1}});
    }

    //Finish loop
    render(window);
    handleInput(window);
  }
}
