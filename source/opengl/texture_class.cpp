#include "texture_class.h"
#include <stb_image.h>
#include <stdexcept>

inline GLenum getFormat(int channel_count){
  switch(channel_count){
    case 1: return GL_RED;
    case 3: return GL_RGB;
    default: return GL_RGBA;
  }
}

Texture Texture::Color(unsigned color){
  return Texture::Image(1, 1, &color, GL_RGBA, GL_CLAMP_TO_EDGE);
}

Texture Texture::Image(const void* a_data, size_t a_width, GLenum wrap_type){
  //Load image
  int width, height, channel_count;
  unsigned char* data = stbi_load_from_memory((unsigned char*)a_data, a_width, &width, &height, &channel_count, 0);
  if(!data) throw std::runtime_error("Failed to load texture from memory");

  GLenum format = getFormat(channel_count);
  Texture texture = Image(width, height, data, format, wrap_type);

  //Unload image
  stbi_image_free(data);

  return texture;
}

Texture Texture::Image(GLsizei width, GLsizei height, const void* data, GLenum format, GLenum wrap_type){
  Texture texture;
  texture.target_type = GL_TEXTURE_2D;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);

  if(wrap_type){
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_type);
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return texture;
}

Texture Texture::Attachment(GLsizei width, GLsizei height, GLenum internal_format, GLenum format, GLenum data_type, size_t sample_count){
  Texture texture;
  if(sample_count > 1){
    texture.target_type = GL_TEXTURE_2D_MULTISAMPLE;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture.id);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, sample_count, internal_format, width, height, GL_TRUE);
  }
  else{
    texture.target_type = GL_TEXTURE_2D;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, data_type, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  return texture;
}

