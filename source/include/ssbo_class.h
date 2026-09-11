#pragma once
#include "base_buffer.hpp"

struct SSBODeleter {
  void operator()(GLuint id) const {
    glDeleteBuffers(1, &id);
  }
};

class SSBO : public BaseBuffer<SSBODeleter> {
public:
  SSBO(){
    glGenBuffers(1, &id);
  }

  template<typename T>
  void setData(const std::vector<T>& data){
    bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(T), data.data(), GL_DYNAMIC_DRAW);
  }

  void bind() const {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
  }

  void bindBase(GLuint base) const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, base, id);
  }

  GLuint getId() const {
    return id;
  }
};

