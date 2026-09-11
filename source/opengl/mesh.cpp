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

