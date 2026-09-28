#include "engine.h"
#include "assimp.h"

namespace Engine {
  std::map<std::string, Model, std::less<> > models;

  //Creates model and caches it
  const Model& getModel(std::string_view path){
    auto it = models.lower_bound(path);
    if(it == models.end() || (path < it->first))
      it = models.emplace_hint(it, std::string(path), Assimp::loadModel(path.data()));
    return it->second;
  }
}
