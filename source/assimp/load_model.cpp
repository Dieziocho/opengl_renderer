#include "assimp.h"
#include "assimp/mesh.h"
#include "data_buffer.h"
#include "glm/ext/matrix_transform.hpp"
#include "texture_class.h"
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <GL/gl.h>
#include <map>
#include <stdexcept>
using Path = std::filesystem::path;
using TextureMap = std::map<std::string, std::shared_ptr<const Texture> >;

glm::mat4 convertMatrix(const aiMatrix4x4& m){
  return glm::mat4(
    m.a1, m.b1, m.c1, m.d1,
    m.a2, m.b2, m.c2, m.d2,
    m.a3, m.b3, m.c3, m.d3,
    m.a4, m.b4, m.c4, m.d4
    );
}

glm::mat4 operator*(const glm::mat4& left, const aiMatrix4x4& m){
  return left * convertMatrix(m);
}

glm::vec3 convertVertex(const aiVector3D& vertex){
  return {vertex.x, vertex.y, vertex.z};
}

void recollectBoneNames(Model& model, const aiMesh& mesh){
  for(unsigned i = 0; i < mesh.mNumBones; ++i){
    std::string bone_name = mesh.mBones[i]->mName.C_Str();
    if(bone_name.ends_with("_end")) continue;
    if(model.bones.find(bone_name) == model.bones.end()){
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
        glm::vec4(convertVertex(mesh.mVertices[i]), 1.0f),
        has_texture_coords
          ? convertVertex(mesh.mTextureCoords[0][i])
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

unsigned loadBones(Model& model, const aiScene& scene){
  size_t bone_count = model.bones.size();
  model.bones.resize();
  model.bones_transforms.reserve<glm::mat4>(bone_count);

  for(auto& [bone_name, bone_id] : model.bones.map()){
    const aiBone* bone = scene.findBone(aiString(bone_name.data()));
    const aiNode* node = scene.mRootNode->findBoneNode(bone);
    if(!bone || !node) continue;

    model.bones[bone_id].offset = convertMatrix(bone->mOffsetMatrix);
    for(unsigned i = 0; i < node->mNumChildren; ++i){
      std::string child_name = node->mChildren[i]->mName.C_Str();
      if(child_name.ends_with("_end")) continue;

      unsigned child_id = model.bones.getId(child_name);
      model.bones[child_id].parent = bone_id;
      model.bones[bone_id].children.push_back(child_id);
    }
  }

  for(unsigned i = 0; i < model.bones.size(); ++i){
    Bone& bone = model.bones[i];
    if(bone.parent == -1u) return i;
  }
  return -1;
}

Model Assimp::loadModel(const Path& file_path, unsigned flags){
  if(!exists(file_path)) throw std::runtime_error(file_path.string() + ": does not exist");

  importer.SetPropertyInteger(
    AI_CONFIG_PP_RVC_FLAGS,
    aiComponent_CAMERAS |
    aiComponent_COLORS |
    aiComponent_LIGHTS |
    aiComponent_TANGENTS_AND_BITANGENTS
    );

  //Load file
  const aiScene* scene = importer.ReadFile(
    file_path,
    flags |
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
  loadBones(model, *scene);
  model.transform = 1;
  model.model_id = Model::current_model_id++;

  importer.FreeScene();

  return model;
}
