#pragma once
#include "base_buffer.h"
#include <vector>
#include <cassert>

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
  void reserve(size_t count){
    buffer_size = count * sizeof(T);

    bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, buffer_size, nullptr, GL_DYNAMIC_DRAW);
  }

  template<typename T>
  void setData(const std::vector<T>& data){
    buffer_size = data.size() * sizeof(T);

    bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, buffer_size, data.data(), GL_DYNAMIC_DRAW);
  }

  template<typename T>
  void subData(const std::vector<T>& data){
    assert((data.size() * sizeof(T)) == buffer_size);

    bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, buffer_size, data.data());
  }

  void bind() const {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
  }

  void bindBase(unsigned base) const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, base, id);
  }

  GLuint getId() const {
    return id;
  }

private:
  size_t buffer_size = 0;
};
