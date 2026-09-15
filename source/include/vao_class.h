#pragma once
#include "base_buffer.h"
#include "bone_info.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "vertex.h"

struct VaoDeleter {
  void operator()(GLuint id) const {
    glDeleteVertexArrays(1, &id);
  }
};

class VAO : public BaseBuffer<VaoDeleter> {
public:
  VAO(){
    glGenVertexArrays(1, &id);
  }

  void addAttribute(size_t count, unsigned type, size_t stride, size_t offset){
    glEnableVertexAttribArray(attribute_index);
    glVertexAttribPointer(attribute_index, count, type, GL_FALSE, stride, (void*)offset);
    attribute_index++;
  }

  template<typename T>
  void addAttribute(){
    if constexpr(std::is_same_v<T, VertexData<2> >){
      addBasicAttribute<float>(sizeof(VertexData<2>), offsetof(Vertex2, position), 2);
      addBasicAttribute<float>(sizeof(VertexData<2>), offsetof(Vertex2, texture_coordinates), 2);
    }
    else if constexpr(std::is_same_v<T, VertexData<3> >){
      addBasicAttribute<float>(sizeof(VertexData<3>), offsetof(Vertex3, position), 3);
      addBasicAttribute<float>(sizeof(VertexData<3>), offsetof(Vertex3, texture_coordinates), 2);
    }
    else if constexpr(std::is_same_v<T, BoneVertexInfo>){
      addBasicAttribute<float>(sizeof(BoneVertexInfo), offsetof(BoneVertexInfo, bone_ids), 4);
      addBasicAttribute<float>(sizeof(BoneVertexInfo), offsetof(BoneVertexInfo, weights), 4);
    }
    else
      addBasicAttribute<T>();
  }

  template<typename T>
  void addBasicAttribute(size_t stride = sizeof(T), size_t offset = 0, size_t count = 1){
    glEnableVertexAttribArray(attribute_index);
    if(false);
    else if constexpr(std::is_same_v<T, int>)      glVertexAttribIPointer(attribute_index, count * 1, GL_INT, stride, (void*)offset);
    else if constexpr(std::is_same_v<T, float>)     glVertexAttribPointer(attribute_index, count * 1, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    else if constexpr(std::is_same_v<T, glm::vec2>) glVertexAttribPointer(attribute_index, count * 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    else if constexpr(std::is_same_v<T, glm::vec3>) glVertexAttribPointer(attribute_index, count * 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    else
      static_assert(false, "Invalid attribute type");

    attribute_index++;
  }

  void bind() const {
    glBindVertexArray(id);
  }

private:
  unsigned attribute_index = 0;
};

