#include "engine.h"

void setups(){
  const Model& model = Engine::getModel("resources/Helios/helios.fbx");
  for(unsigned i = 0; i < 10; ++i){
    auto* entity = &Engine::createEntity(model);
    entity->translate({0,i,0});
    i % 2 ?
    entity->playAnimation("Hi") :
    entity->playAnimation("Open");
  }
}

void onFrame(){
}
