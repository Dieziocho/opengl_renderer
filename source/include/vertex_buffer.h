#pragma once
#include "vertex.h"
#include "vao_class.h"
#include "vbo_class.h"
#include "ebo_class.h"

class VertexBuffer {
public:
  template<typename T>
  using Vector = const std::vector<T>;

  VertexBuffer() = default;

  template<size_t count>
  VertexBuffer(Vector<VertexData<count> >& vertices, Vector<unsigned> indices){
    index_count = indices.size();

    vao.bind();
    vbo.load(vertices);
    ebo.load(indices);

    using Vertex = VertexData<count>;
    vao.addAttribute<typename Vertex::vec>(sizeof(Vertex), offsetof(Vertex, position));
    vao.addAttribute<glm::vec2>(sizeof(Vertex), offsetof(Vertex, texture_coordinates));

    mode = GL_TRIANGLES;
  }

  static VertexBuffer vector2(Vector<Vertex2 >& vertices, Vector<unsigned> indices){
    return VertexBuffer(vertices, indices);
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
