#include "engine.h"

Entity* helios;

void setups(){
  const Model& model = Engine::getModel("resources/Helios/helios.fbx");
  helios = &Engine::createEntity(model);
  helios->playAnimation("Hi");
}

void onFrame(){
  helios->translate({0,0.001,0});
}
