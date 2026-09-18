#pragma once
#include "base_buffer.h"
#include "bone.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "vertex.h"
#include <bitset>

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

  template<typename T>
  void setAttribute(unsigned index){
    if constexpr(std::is_same_v<T, VertexData<2> >){
      setBasicAttribute<float>(index++, sizeof(VertexData<2>), offsetof(Vertex2, position), 2);
      setBasicAttribute<float>(index++, sizeof(VertexData<2>), offsetof(Vertex2, texture_coordinates), 2);
    }
    else if constexpr(std::is_same_v<T, VertexData<3> >){
      setBasicAttribute<float>(index++, sizeof(VertexData<3>), offsetof(Vertex3, position), 3);
      setBasicAttribute<float>(index++, sizeof(VertexData<3>), offsetof(Vertex3, texture_coordinates), 2);
    }
    else if constexpr(std::is_same_v<T, BoneData>){
      setBasicAttribute<float>(index++, sizeof(BoneData), offsetof(BoneData, bone_ids), 4);
      setBasicAttribute<float>(index++, sizeof(BoneData), offsetof(BoneData, weights), 4);
    }
    else
      setBasicAttribute<T>(index++);
  }

  template<typename T>
  void setBasicAttribute(unsigned index, size_t stride = sizeof(T), size_t offset = 0, size_t count = 1){
    assert(!enabled_attributes[index]);
    enabled_attributes.set(index);

    glEnableVertexAttribArray(index);
    if(false);
    else if constexpr(std::is_same_v<T, int>)      glVertexAttribIPointer(index, count * 1, GL_INT, stride, (void*)offset);
    else if constexpr(std::is_same_v<T, float>)     glVertexAttribPointer(index, count * 1, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    else if constexpr(std::is_same_v<T, glm::vec2>) glVertexAttribPointer(index, count * 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    else if constexpr(std::is_same_v<T, glm::vec3>) glVertexAttribPointer(index, count * 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    else
      static_assert(false, "Invalid attribute type");
  }

  void bind() const {
    glBindVertexArray(id);
  }

private:
  std::bitset<32> enabled_attributes;
};

