#include "engine.h"
#include "entity.h"

namespace Engine {
  std::vector<Entity> entities;

  Entity& createEntity(const Model& model){
    entities.push_back(Entity(model));
    return entities.back();
  }

  std::vector<Entity>& getEntities(){
    return entities;
  }
}
