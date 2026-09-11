#pragma once
#include "opengl.h"

template<typename Deleter>
class BaseBuffer {
public:
  BaseBuffer() = default;
  BaseBuffer(const BaseBuffer&) = delete;
  BaseBuffer& operator=(const BaseBuffer&) = delete;

  BaseBuffer(BaseBuffer&& other) noexcept : id(other.id){
    other.id = 0;
  }

  BaseBuffer& operator=(BaseBuffer&& other) noexcept {
    if(this == &other)
      return *this;

    reset();

    id = other.id;
    other.id = 0;

    return *this;
  }

  ~BaseBuffer(){
    reset();
  }

protected:
  GLuint id = 0;

  void reset(){
    if(!id) return;
    if(!glfwGetCurrentContext()) return;
    Deleter()(id);
    id = 0;
  }
};
