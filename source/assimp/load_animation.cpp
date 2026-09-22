#include "assimp.h"
#include "assimp/anim.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/ext/quaternion_float.hpp"

glm::vec3 convertVec3(const aiVector3D& v){
  return glm::vec3(v.x, v.y, v.z);
}

glm::quat convertQuat(const aiQuaternion& q){
  return glm::quat(q.w, q.x, q.y, q.z);
}

void loadBoneAnimations(Animation& animation, const aiAnimation& ai_animation){
  animation.ticks_per_second = ai_animation.mTicksPerSecond ? ai_animation.mTicksPerSecond : 25;
  animation.duration = ai_animation.mDuration;

  for(auto* channel : std::span(ai_animation.mChannels, ai_animation.mNumChannels)){
    std::string bone_name = channel->mNodeName.C_Str();

    for(auto& position_key : std::span(channel->mPositionKeys, channel->mNumPositionKeys)){
      KeyPosition data;
      data.value = convertVec3(position_key.mValue);
      data.time = position_key.mTime;
      animation.bones[bone_name].positions.push_back(data);
    }

    for(auto& rotation_key : std::span(channel->mRotationKeys, channel->mNumRotationKeys)){
      KeyRotation data;
      data.value = convertQuat(rotation_key.mValue);
      data.time = rotation_key.mTime;
      animation.bones[bone_name].rotations.push_back(data);
    }

    for(auto& scale_key : std::span(channel->mScalingKeys, channel->mNumScalingKeys)){
      KeyScale data;
      data.value = convertVec3(scale_key.mValue);
      data.time = scale_key.mTime;
      animation.bones[bone_name].scales.push_back(data);
    }
  }
}

Animation Assimp::loadAnimation(const Path& file_path, unsigned flags){
  if(!exists(file_path)) throw std::runtime_error(file_path.string() + ": does not exist");

  importer.SetPropertyInteger(
    AI_CONFIG_PP_RVC_FLAGS,
    aiComponent_NORMALS |
    aiComponent_TANGENTS_AND_BITANGENTS |
    aiComponent_COLORS |
    aiComponent_TEXCOORDS |
    aiComponent_MATERIALS |
    aiComponent_LIGHTS |
    aiComponent_CAMERAS |
    aiComponent_TEXTURES
    );

  //Load file
  const aiScene* scene = importer.ReadFile(
    file_path,
    flags |
    aiProcess_RemoveComponent | aiProcess_ImproveCacheLocality | aiProcess_SortByPType |
    aiProcess_RemoveRedundantMaterials | aiProcess_FindInstances
    );

  //Check for errors
  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    throw std::runtime_error(std::format("Failed to load file {}: {}", file_path.string(), importer.GetErrorString()));
  if(!scene->HasAnimations())
    throw std::runtime_error(std::format("File {} has no animations", file_path.string()));

  Animation animation;
  loadBoneAnimations(animation, *scene->mAnimations[0]);

  importer.FreeScene();

  return animation;
}
