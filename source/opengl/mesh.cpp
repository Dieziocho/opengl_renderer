#include "mesh.h"
#include "shapes.h"

Mesh Mesh::Square(){
  Mesh mesh;
  mesh.data = std::shared_ptr<DataBuffer>(new DataBuffer(Square::vertices, Square::indices));
  mesh.texture = nullptr;
  mesh.flags = 0;
  return mesh;
}

Mesh Mesh::FromBuffer(DataBuffer&& buffer){
  Mesh mesh;
  mesh.data = std::shared_ptr<DataBuffer>(new DataBuffer(std::move(buffer)));
  return mesh;
}

Mesh Mesh::FromData(Vector<Vertex2>& vertices, Vector<unsigned> indices, std::shared_ptr<const Texture> texture, unsigned flags){
  GLenum mode = flags & MESH_LINES ? GL_LINES : GL_TRIANGLES;

  Mesh mesh;
  mesh.data = std::shared_ptr<DataBuffer>(new DataBuffer(DataBuffer(vertices, indices, mode)));
  mesh.texture_owner = texture;
  mesh.texture = texture.get();
  mesh.flags = flags;
  return mesh;
}

Mesh Mesh::FromTexture(const Texture& texture){
  Mesh mesh;
  mesh.data = std::shared_ptr<DataBuffer>(new DataBuffer(DataBuffer(Square::vertices, Square::indices)));
  mesh.texture = &texture;
  mesh.flags = 0;
  return mesh;
}

Mesh Mesh::FromColor(unsigned color){
  bool is_transparent = ~color & 0xff000000;

  Mesh mesh;
  mesh.data = std::shared_ptr<DataBuffer>(new DataBuffer(DataBuffer(Square::vertices, Square::indices)));
  mesh.texture_owner = std::shared_ptr<Texture>(new Texture(Texture::Color(color)));
  mesh.texture = mesh.texture_owner.get();
  mesh.flags = is_transparent ? MESH_TRANSPARENT : 0;

  return mesh;
}
