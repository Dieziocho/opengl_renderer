#include "render.h"
#include "camera.h"
#include "input.h"
#include "draw.h"
#include "assimp.h"
#include "shaders.h"
#include "ssbo_class.h"
#include "glm.h"

void updateBones(unsigned id, Model& model, std::vector<glm::mat4>& transforms, glm::mat4 accum = 1){
  transforms[id] = accum * glm::inverse(model.bones[id].offset) * transforms[id] * model.bones[id].offset;

  for(auto child : model.bones[id].children)
    updateBones(child, model, transforms, transforms[id]);
};

void Render::mainLoop(GLFWwindow* window){
  Model model = Assimp::loadModel("resources/Helios/helios.fbx");

  SSBO ssbo;
  ssbo.bindBase(0);
  ssbo.reserve<glm::mat4>(model.bones.size());
  std::vector<glm::mat4> transforms(model.bones.size());

  while(!glfwWindowShouldClose(window)){
    Camera::update();

    std::fill(transforms.begin(), transforms.end(), rotate(glfwGetTime(), {1,0,0}));
    updateBones(0, model, transforms);
    ssbo.subData(transforms);

    drawModel(model, Shaders::animated_3d);

    //Finish loop
    render(window);
    handleInput(window);
  }
}
