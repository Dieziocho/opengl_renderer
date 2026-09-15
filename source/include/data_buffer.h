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

    attach(data);
    ebo.load(indices);

    mode = mesh_mode;
  }

  template<typename T>
  void attach(Vector<T>& data){
    vao.bind();
    vbos.push_back({});
    vbos.rbegin()->load(data);

    vao.addAttribute<T>();
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
  std::vector<VBO> vbos;
  VAO vao;
  EBO ebo;
  GLenum mode;

  size_t index_count = 0;
};
