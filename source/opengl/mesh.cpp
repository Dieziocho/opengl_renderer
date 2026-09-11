#include "mesh.h"
#include "shapes.h"
#include "vertex_buffer.h"

Mesh Mesh::Square(){
  Mesh mesh;
  mesh.data = std::shared_ptr<VertexBuffer>(new VertexBuffer(Square::vertices, Square::indices));
  mesh.texture = nullptr;
  mesh.flags = 0;
  return mesh;
}

Mesh Mesh::FromBuffer(VertexBuffer&& buffer){
  Mesh mesh;
  mesh.data = std::shared_ptr<VertexBuffer>(new VertexBuffer(std::move(buffer)));
  return mesh;
}

Mesh Mesh::FromData(Vector<Vertex2>& vertices, Vector<unsigned> indices, std::shared_ptr<const Texture> texture, unsigned flags){
  GLenum mode = flags & MESH_LINES ? GL_LINES : GL_TRIANGLES;

  Mesh mesh;
  mesh.data = std::shared_ptr<VertexBuffer>(new VertexBuffer(VertexBuffer::vector2(vertices, indices, mode)));
  mesh.texture_owner = texture;
  mesh.texture = texture.get();
  mesh.flags = flags;
  return mesh;
}

Mesh Mesh::FromTexture(const Texture& texture){
  Mesh mesh;
  mesh.data = std::shared_ptr<VertexBuffer>(new VertexBuffer(VertexBuffer::vector2(Square::vertices, Square::indices)));
  mesh.texture = &texture;
  mesh.flags = 0;
  return mesh;
}

Mesh Mesh::FromTexture(Texture&& texture){
  Mesh mesh;
  mesh.data = std::shared_ptr<VertexBuffer>(new VertexBuffer(VertexBuffer::vector2(Square::vertices, Square::indices)));
  mesh.texture_owner = std::shared_ptr<Texture>(new Texture(std::move(texture)));
  mesh.texture = mesh.texture_owner.get();
  mesh.flags = 0;
  return mesh;
}

