#pragma once
#include "model_instance.h"

class Entity {
public:
  Entity(const Model& model) : model(model){}

  void playAnimation(const char* animation_name){
    model.playAnimation(animation_name);
  }

  void translate(glm::vec3 offset){
    transform = glm::translate(transform, offset);
  }

  const glm::mat4& getTransform() const {
    return transform;
  }

  ModelInstance& getInstance(){
    return model;
  }

private:
  ModelInstance model;
  glm::mat4 transform = 1;
};
