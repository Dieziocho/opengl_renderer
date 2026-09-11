#pragma once
#include "base_buffer.h"

struct TextureDeleter {
  void operator()(GLuint id) const {
    glDeleteTextures(1, &id);
  }
};

class Texture : public BaseBuffer<TextureDeleter> {
public:
  Texture(){
    glGenTextures(1, &id);
  }

  static Texture Color(unsigned color);
  static Texture Image(const void* data, size_t width, GLenum wrap_type);
  static Texture Image(GLsizei width, GLsizei height, const void* data, GLenum format, GLenum wrap_type);
  static Texture Attachment(GLsizei width, GLsizei height, GLenum internal_format, GLenum format, GLenum data_type, size_t sample_count);

  void bind(unsigned index) const {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(target_type, id);
  }

  GLuint getId() const {
    return id;
  }

  GLenum getTargetType() const {
    return target_type;
  }

private:
  GLenum target_type;
};
