#pragma once
#include "animation.h"
#include <format>

class AnimationList {
public:
  void addAnimation(const std::string& name, Animation&& animation){
    auto true_name = removePrefix(name);
    animation_map[true_name] = animations.size();
    animations.emplace_back(std::move(animation));
  }

  unsigned getId(const char* name) const {
    auto it = animation_map.find(name);
    if(it == animation_map.end())
      throw std::runtime_error(std::format("No animation named {} present", name));

    return it->second;
  }

  const Animation& operator[](const char* name) const {
    return animations[getId(name)];
  }

  const Animation& operator[](unsigned id) const {
    return animations[id];
  }

private:
  std::map<std::string, unsigned> animation_map;
  std::vector<Animation> animations;

  std::string removePrefix(std::string string){
    size_t position = string.find('|');

    if(position != std::string::npos){
      string.erase(0, position + 1);
    }

    return string;
  }
};
