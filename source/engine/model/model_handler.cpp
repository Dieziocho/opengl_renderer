#include "engine.h"
#include "assimp.h"

namespace Engine {
  std::map<std::string, Model, std::less<> > models;

  //Creates model and caches it
  const Model& getModel(std::string_view path, unsigned flags){ //TODO: load as different model if flags are different
    auto it = models.lower_bound(path);
    if(it == models.end() || (path < it->first))
      it = models.emplace_hint(it, std::string(path), Assimp::loadModel(path.data(), flags));
    return it->second;
  }
}
