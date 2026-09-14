#pragma once
#include "vao_class.h"
#include "vbo_class.h"
#include "ebo_class.h"

class DataBuffer {
public:
  template<typename T>
  using Vector = const std::vector<T>;

  DataBuffer() = default;
  DataBuffer(DataBuffer&&) = default;

  template<typename T>
  DataBuffer(Vector<T>& data, Vector<unsigned> indices, GLenum mesh_mode = GL_TRIANGLES){
    index_count = indices.size();

    vao.bind();
    vbo.load(data);
    ebo.load(indices);

    vao.addAttribute<T>(sizeof(T), 0);

    mode = mesh_mode;
  }

  void bind() const {
    vao.bind();
  }

  size_t size() const {
    return index_count;
  }

  size_t getMode() const {
    return mode;
  }

private:
  VAO vao;
  VBO vbo;
  EBO ebo;
  GLenum mode;

  size_t index_count = 0;
};
