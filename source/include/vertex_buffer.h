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
  VertexBuffer(VertexBuffer&&) = default;

  template<typename T>
  VertexBuffer(Vector<T>& data, Vector<unsigned> indices, GLenum mesh_mode = GL_TRIANGLES){
    index_count = indices.size();

    vao.bind();
    vbo.load(data);
    ebo.load(indices);

    vao.addAttribute<T>(sizeof(T), 0);

    mode = mesh_mode;
  }

  template<size_t count>
  VertexBuffer(Vector<VertexData<count> >& vertices, Vector<unsigned> indices, GLenum mesh_mode = GL_TRIANGLES){
    index_count = indices.size();

    vao.bind();
    vbo.load(vertices);
    ebo.load(indices);

    using Vertex = VertexData<count>;
    vao.addAttribute<typename Vertex::vec>(sizeof(Vertex), offsetof(Vertex, position));
    vao.addAttribute<glm::vec2>(sizeof(Vertex), offsetof(Vertex, texture_coordinates));

    mode = mesh_mode;
  }

  static VertexBuffer vector2(Vector<Vertex2 >& vertices, Vector<unsigned> indices, GLenum mesh_mode = GL_TRIANGLES){
    return VertexBuffer(vertices, indices, mesh_mode);
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
