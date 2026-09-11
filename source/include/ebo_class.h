#pragma once
#include "base_buffer.h"
#include <vector>

struct EboDeleter {
  void operator()(GLuint id) const {
    glDeleteBuffers(1, &id);
  }
};

class EBO : public BaseBuffer<EboDeleter> {
public:
  EBO(){
    glGenBuffers(1, &id);
  }

  EBO(const std::vector<unsigned>& data) : EBO(){
    load(data);
  }

  void load(const std::vector<unsigned>& data){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * data.size(), data.data(), GL_STATIC_DRAW);
  }
};

