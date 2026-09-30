#include "engine.h"

void setups(){
  const Model& model = Engine::getModel("resources/Helios/helios.fbx", IGNORE_ANIMATIONS);
  for(unsigned i = 0; i < 1000; ++i){
    auto* entity = &Engine::createEntity(model);
    entity->translate({0,i,0});
  }
}

void onFrame(){
}
