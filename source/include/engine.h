#pragma once
#include "model.h"
#include "entity.h"
#define IGNORE_ANIMATIONS (1 << 0)

namespace Engine {
  const Model& getModel(std::string_view path, unsigned flags = 0);
  Entity& createEntity(const Model& model);


  std::vector<Entity>& getEntities();
}
