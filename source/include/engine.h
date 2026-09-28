#pragma once
#include "model.h"
#include "entity.h"

namespace Engine {
  const Model& getModel(std::string_view path);
  Entity& createEntity(const Model& model);


  std::vector<Entity>& getEntities();
}
