#pragma once
#include "base_buffer.h"
#include <vector>

struct VboDeleter {
  void operator()(GLuint id) const {
    glDeleteBuffers(1, &id);
  }
};

class VBO : public BaseBuffer<VboDeleter> {
public:
  VBO(){
    glGenBuffers(1, &id);
  }

  template<typename T>
  VBO(const std::vector<T>& data) : VBO(){
    load(data);
  }

  template<typename T>
  void load(const std::vector<T>& data){
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(), GL_STATIC_DRAW);
  }
};
