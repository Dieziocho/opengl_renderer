#pragma once
#include "shader_class.h"
#include "data_buffer.h"
#include "texture_class.h"
#define MESH_TWO_SIDED (1 << 0)
#define MESH_TRANSPARENT (1 << 1)
#define MESH_LINES (1 << 2)

class Mesh {
public:
  template<typename T>
  using Vector = const std::vector<T>;

  static Mesh Square();

  static Mesh FromBuffer(DataBuffer&& buffer, std::shared_ptr<const Texture> texture, unsigned flags);
  static Mesh FromData(Vector<Vertex2>& vertices, Vector<unsigned> indices, std::shared_ptr<const Texture> texture, unsigned flags);
  static Mesh FromData(Vector<Vertex3>& vertices, Vector<unsigned> indices, std::shared_ptr<const Texture> texture, unsigned flags);
  static Mesh FromTexture(const Texture& texture);
  static Mesh FromTexture(Texture&& texture);
  static Mesh FromColor(unsigned color);

  void addFlags(unsigned n){
    flags |= n;
  }

  void bind(Shader& shader) const {
    data->bind();
    if(texture) texture->bind(0);
    shader.setUniform("mesh_texture", 0);
  }

  size_t size() const {
    return data->size();
  }

  unsigned getFlags() const {
    return flags;
  }

  unsigned getMode() const {
    return data->getMode();
  }

private:
  std::shared_ptr<const DataBuffer> data;
  std::shared_ptr<const Texture> texture_owner;
  const Texture* texture;
  unsigned flags = 0;
};
