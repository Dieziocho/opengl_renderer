#pragma once
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/quaternion.hpp"
#include <format>
#include <stdexcept>
#include <string>
#include <map>
#define BONE_COUNT glm::ivec4::length()

struct BoneData {
  //Bone indexes which will influence this vertex
  int bone_ids[BONE_COUNT] = {-1, -1, -1, -1};
  //Weights from each bone
  float weights[BONE_COUNT] = {0, 0, 0, 0};
};

struct Bone {
  unsigned parent = -1u;
  std::vector<unsigned> children;
  glm::mat4 offset;

  glm::vec3 position = glm::vec3(0,0,0);
  glm::quat rotation = glm::quat(1,0,0,0);
  glm::vec3 scale = glm::vec3(1,1,1);

  glm::mat4 getTransform(){
    return glm::translate(glm::mat4(1), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1), scale);
  }
};

class Bones {
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

  Bone* find(const std::string& name){
    auto it = bone_map.find(name);
    if(it == bone_map.end()) return end();
    return &bones[it->second];
  }

  Bone* begin(){
    return bones.data();
  }

  Bone* end(){
    return bones.data() + bones.size();
  }

  size_t size(){
    return bone_map.size();
  }

  unsigned getId(const std::string& name){
    auto it = bone_map.find(name);
    if(it == bone_map.end()) throw std::runtime_error(std::format("No bone named {} exists", name));
    return it->second;
  }

  unsigned getId(const Bone* bone){
    unsigned index = bone - bones.data();
    if(index >= bones.size()) throw std::runtime_error("Invalid bone pointer");
    return index;
  }

  Bone& operator[](unsigned index){
    return bones[index];
  }

  Bone& operator[](const std::string& name){
    auto it = bone_map.find(name);
    if(it == bone_map.end()) throw std::runtime_error(std::format("No bone named {} exists", name));
    return bones[it->second];
  }

  const std::map<std::string, unsigned>& map(){
    return bone_map;
  }

private:
  std::map<std::string, unsigned> bone_map;
  std::vector<Bone> bones;
};

