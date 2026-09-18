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
  void attach(unsigned index, Vector<T>& data){
    vao.bind();
    vbos.push_back({});
    vbos.rbegin()->load(data);

    vao.setAttribute<T>(index);
  }

  void setIndices(Vector<unsigned> indices){
    index_count = indices.size();

    vao.bind();
    ebo.load(indices);
  }

  void bind() const {
    vao.bind();
  }

  size_t size() const {
    return index_count;
  }

private:
  std::vector<VBO> vbos;
  VAO vao;
  EBO ebo;

  size_t index_count = 0;
};
