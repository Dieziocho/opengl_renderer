#include "assimp.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"
#include "assimp/texture.h"
#include <stdexcept>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
using Path = std::filesystem::path;
using TextureMap = std::map<std::string, std::shared_ptr<const Texture> >;


glm::mat4 operator*(const glm::mat4& left, const aiMatrix4x4& m){
  return left * Assimp::convertMat4(m);
}

void recollectBoneNames(Model& model, const aiMesh& mesh){
  for(unsigned i = 0; i < mesh.mNumBones; ++i){
    std::string bone_name = mesh.mBones[i]->mName.C_Str();
    if(!model.bones.contains(bone_name)){
      model.bones.registerBone(bone_name);
    }
  }
}

void setVertexBoneData(BoneData& bone_data, int boneID, float weight){
  for(int i = 0; i < BONE_COUNT; ++i){
    if(bone_data.bone_ids[i] < 0){
      bone_data.weights[i] = weight;
      bone_data.bone_ids[i] = boneID;
      break;
    }
  }
}

void loadVertexBoneweights(Model& model, std::vector<BoneData>& bone_data, aiMesh& mesh){
  for(unsigned i = 0; i < mesh.mNumBones; ++i){
    aiString& ai_bone_name = mesh.mBones[i]->mName;
    std::string bone_name = ai_bone_name.C_Str();

    unsigned bone_id = model.bones.getId(bone_name);
    auto* weights = mesh.mBones[i]->mWeights;
    unsigned weight_count = mesh.mBones[i]->mNumWeights;
    for(auto& weight : std::span(weights, weight_count)){
      unsigned vertex_id = weight.mVertexId;
      float weight_value = weight.mWeight;
      assert(vertex_id <= bone_data.size());
      setVertexBoneData(bone_data[vertex_id], bone_id, weight_value);
    }
  }
}

Texture* loadTexture(const char* texture_name, const aiScene& scene){
  unsigned index = std::stoi(texture_name + 1);
  aiTexture* ai_texture = scene.mTextures[index];

  //Compressed embedded file
  if(ai_texture->mHeight == 0)
    return new Texture(Texture::Image(ai_texture->pcData, ai_texture->mWidth, GL_REPEAT));

  //Raw embedded file
  return new Texture(Texture::Image(ai_texture->mWidth, ai_texture->mHeight, ai_texture->pcData, GL_BGRA, GL_REPEAT));
}

std::shared_ptr<const Texture> loadMaterialTexture(const aiScene& scene, const aiMaterial& material, TextureMap& textures){
  if(!material.GetTextureCount(aiTextureType_DIFFUSE)) return nullptr;
  aiString texture_aiString;
  material.GetTexture(aiTextureType_DIFFUSE, 0, &texture_aiString);

  const char* texture_name = texture_aiString.C_Str();

  //Skip if already loaded
  auto it = textures.find(texture_name);
  if(it != textures.end()){
    return it->second;
  }

  auto texture = std::shared_ptr<Texture>(loadTexture(texture_name, scene));
  textures[texture_name] = texture;
  return texture;
}

void loadMeshes(Model& model, const aiScene& scene){
  auto* meshes = scene.mMeshes;
  size_t count = scene.mNumMeshes;
  model.meshes.reserve(scene.mNumMeshes);

  TextureMap textures;

  for(unsigned i = 0; i < count; ++i){
    auto& mesh = *meshes[i];
    auto& material = *scene.mMaterials[mesh.mMaterialIndex];
    bool has_texture_coords = mesh.HasTextureCoords(0);

    //Load vertices
    std::vector<Vertex3> vertices;
    vertices.reserve(mesh.mNumVertices);
    for(unsigned i = 0; i < mesh.mNumVertices; ++i)
      vertices.emplace_back(Vertex3{
        glm::vec4(Assimp::convertVec3(mesh.mVertices[i]), 1.0f),
        has_texture_coords
          ? Assimp::convertVec3(mesh.mTextureCoords[0][i])
          : glm::vec2{0, 0}
      });

    //Load indices
    std::vector<unsigned> indices;
    indices.reserve(mesh.mNumFaces * 3);
    for(auto& face : std::span(mesh.mFaces, mesh.mNumFaces))
      for(auto& index : std::span(face.mIndices, face.mNumIndices))
        indices.push_back(index);

    //Get bone names via mesh reference
    recollectBoneNames(model, mesh);
    std::vector<BoneData> bone_data(vertices.size());
    loadVertexBoneweights(model, bone_data, mesh);

    //Load texture
    auto texture = loadMaterialTexture(scene, material, textures);

    //Get flags
    unsigned flags = 0;
    int two_sided = 0;
    aiString alpha_mode;
    if((material.Get(AI_MATKEY_TWOSIDED, two_sided) == AI_SUCCESS) && two_sided){
      flags |= MESH_TWO_SIDED;
    }
    if(material.Get("$mat.gltf.alphaMode", 0, 0, alpha_mode) == AI_SUCCESS){
      if(alpha_mode == aiString("BLEND"))
        flags |= MESH_TRANSPARENT;
      else if(alpha_mode == aiString("MASK")){}
    }

    DataBuffer buffer;
    buffer.attach(0, vertices);
    buffer.attach(2, bone_data);
    buffer.setIndices(indices);

    model.meshes.emplace_back(Mesh::FromBuffer(std::move(buffer), texture, flags));
  }
}

void loadInstances(Model& model, const aiNode* node, glm::mat4 transform = glm::mat4(1)){
  transform = transform * node->mTransformation;

  for(unsigned i = 0; i < node->mNumMeshes; ++i){
    model.mesh_instances.push_back({node->mMeshes[i], transform});
  }

  for(unsigned i = 0; i < node->mNumChildren; ++i)
    loadInstances(model, node->mChildren[i], transform);
}

void loadBoneMatrices(Model& model, unsigned bone_id, const aiBone& ai_bone, const aiNode& node){
  auto& bone = model.bones[bone_id];
  bone.offset = Assimp::convertMat4(ai_bone.mOffsetMatrix);

  glm::vec3 position;
  glm::quat rotation;
  glm::vec3 scale;
  glm::vec3 skew;
  glm::vec4 perspective;

  bool success = glm::decompose(Assimp::convertMat4(node.mTransformation), scale, rotation, position, skew, perspective);
  if(!success) throw std::runtime_error("Failed to get bone pose");
  bone.default_state.position = position;
  bone.default_state.rotation = rotation;
  bone.default_state.scale = scale;
}

void loadBoneAnimations(Model& model, const aiAnimation& ai_animation){
  Animation animation;
  animation.ticks_per_second = ai_animation.mTicksPerSecond ? ai_animation.mTicksPerSecond : 25;
  animation.duration = ai_animation.mDuration;

  for(auto* channel : std::span(ai_animation.mChannels, ai_animation.mNumChannels)){
    std::string bone_name = channel->mNodeName.C_Str();

    for(auto& position_key : std::span(channel->mPositionKeys, channel->mNumPositionKeys)){
      KeyPosition data;
      data.value = Assimp::convertVec3(position_key.mValue);
      data.time = position_key.mTime;
      animation.bones[bone_name].positions.push_back(data);
    }

    for(auto& rotation_key : std::span(channel->mRotationKeys, channel->mNumRotationKeys)){
      KeyRotation data;
      data.value = Assimp::convertQuat(rotation_key.mValue);
      data.time = rotation_key.mTime;
      animation.bones[bone_name].rotations.push_back(data);
    }

    for(auto& scale_key : std::span(channel->mScalingKeys, channel->mNumScalingKeys)){
      KeyScale data;
      data.value = Assimp::convertVec3(scale_key.mValue);
      data.time = scale_key.mTime;
      animation.bones[bone_name].scales.push_back(data);
    }
  }

  model.animations.addAnimation(ai_animation.mName.C_Str(), std::move(animation));
}

void loadAnimations(Model& model, const aiScene& scene){
  if(!scene.HasAnimations()) return;

  for(auto& animation : std::span(scene.mAnimations, scene.mNumAnimations))
    loadBoneAnimations(model, *animation);
}

unsigned loadBones(Model& model, const aiScene& scene){
  model.bones.resize();

  for(auto& [bone_name, bone_id] : model.bones.map()){
    const aiBone* bone = scene.findBone(aiString(bone_name.data()));
    const aiNode* node = scene.mRootNode->findBoneNode(bone);
    if(!bone || !node) continue;

    loadBoneMatrices(model, bone_id, *bone, *node);
    for(unsigned i = 0; i < node->mNumChildren; ++i){
      std::string child_name = node->mChildren[i]->mName.C_Str();

      auto child = model.bones.find(child_name);
      if(child == model.bones.end()) continue;

      unsigned child_id = model.bones.getId(child);
      model.bones[child_id].parent = bone_id;
      model.bones[bone_id].children.push_back(child_id);
    }
  }

  for(unsigned i = 0; i < model.bones.size(); ++i){
    BoneInfo& bone = model.bones[i];
    if(bone.parent == -1u) return i;
  }
  return -1;
}

Model Assimp::loadModel(const Path& file_path, unsigned){
  if(!exists(file_path)) throw std::runtime_error(file_path.string() + ": does not exist");

  importer.SetPropertyInteger(
    AI_CONFIG_PP_RVC_FLAGS,
    aiComponent_NORMALS |
    aiComponent_TANGENTS_AND_BITANGENTS |
    aiComponent_COLORS |
    aiComponent_LIGHTS |
    aiComponent_CAMERAS
    );

  //Load file
  const aiScene* scene = importer.ReadFile(
    file_path,
    aiProcess_RemoveComponent | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_SortByPType |
    aiProcess_RemoveRedundantMaterials | aiProcess_FindInstances | aiProcess_EmbedTextures
    );

  //Check for errors
  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    throw std::runtime_error(std::format("Failed to load file {}: {}", file_path.string(), importer.GetErrorString()));

  glm::mat4 input_transform =
    file_path.extension() == ".fbx" ?
    glm::scale(glm::mat4(1), glm::vec3(1) * 0.01f) : 1;

  Model model;
  loadMeshes(model, *scene);
  loadInstances(model, scene->mRootNode, input_transform);
  loadAnimations(model, *scene);

  model.root_id = loadBones(model, *scene);
  model.model_id = Model::current_model_id++;

  importer.FreeScene();

  return model;
}
