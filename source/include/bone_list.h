#pragma once
#include "bone.h"
#include <map>
#include <string>
#include <stdexcept>
#include <format>

class BoneList {
public:
  void registerBone(const std::string& name){
    bone_map[name] = bone_map.size();
  }

  void resize(){
    bones.resize(bone_map.size());
  }

  bool contains(const std::string& name){
    auto it = bone_map.find(name);
    return it != bone_map.end();
  }

  BoneInfo* find(const std::string& name){
    auto it = bone_map.find(name);
    if(it == bone_map.end()) return end();
    return &bones[it->second];
  }

  BoneInfo* begin(){
    return bones.data();
  }

  BoneInfo* end(){
    return bones.data() + bones.size();
  }

  const BoneInfo* begin() const {
    return bones.data();
  }

  const BoneInfo* end() const {
    return bones.data() + bones.size();
  }

  size_t size() const {
    return bone_map.size();
  }

  unsigned getId(const std::string& name){
    auto it = bone_map.find(name);
    if(it == bone_map.end()) throw std::runtime_error(std::format("No bone named {} exists", name));
    return it->second;
  }

  unsigned getId(const BoneInfo* bone){
    unsigned index = bone - bones.data();
    if(index >= bones.size()) throw std::runtime_error("Invalid bone pointer");
    return index;
  }

  BoneInfo& operator[](unsigned index){
    return bones[index];
  }

  const BoneInfo& operator[](unsigned index) const {
    return bones[index];
  }

  BoneInfo& operator[](const std::string& name){
    auto it = bone_map.find(name);
    if(it == bone_map.end()) throw std::runtime_error(std::format("No bone named {} exists", name));
    return bones[it->second];
  }

  const std::map<std::string, unsigned>& map() const {
    return bone_map;
  }

private:
  std::map<std::string, unsigned> bone_map;
  std::vector<BoneInfo> bones;
};
