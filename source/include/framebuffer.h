#pragma once
#include "rbo_class.h"
#include "texture_class.h"
#include "config.h"
#include <vector>
#include <memory>
#include <numeric>
#include <stdexcept>

struct FramebufferDeleter {
  void operator()(GLuint id) const {
    glDeleteFramebuffers(1, &id);
  }
};

class Framebuffer : BaseBuffer<FramebufferDeleter> {
public:
  Framebuffer() = default;
  Framebuffer(size_t initial_count, bool add_depth = true, size_t sample_count = 1, size_t width = window_width, size_t height = window_height) :
    width(width), height(height){
    //Create framebuffer object
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    for(unsigned i = 0; i < initial_count; ++i) attachColor(sample_count);

    if(!add_depth) return;

    //Create depth and stencil buffers
    rbo = std::shared_ptr<RBO>(new RBO(sample_count, width, height));
    rbo->attach();

    if(initial_count)
      if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Failed to create framebuffer");
  }

  //Create framebuffer color attachments
  const Texture& attachColor(unsigned sample_count = 1, unsigned internal_format = GL_RGBA, unsigned texture_format = GL_RGBA, unsigned data_type = GL_UNSIGNED_BYTE){
    unsigned index = attachments.size();
    auto& texture = attachments.emplace_back(
      Texture::Attachment(width, height, internal_format, texture_format, data_type, sample_count)
      );

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture.getTargetType(), texture.getId(), 0);
    setDrawBuffers();

    return texture;
  }

  void attachDepthBuffer(const std::shared_ptr<RBO>& depth_buffer){
    rbo = depth_buffer;
    rbo->attach();
  }

  void check(){
    bind();
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      throw std::runtime_error("Failed to create framebuffer");
  }

  void clear(){
    bind();
    clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }

  void clearBuffer(unsigned index, bool set){
    bind();
    glClearBufferfv(GL_COLOR, index, set ? one : zero);
  }

  void bind(){
    bind(id);
  }

  unsigned getId() const {
    return id;
  }

  const Texture& getTexture(unsigned index) const {
    return attachments[index];
  }

  const std::shared_ptr<RBO>& getDepthBuffer(){
    return rbo;
  }

  static void clear(GLuint buffers){
    glClear(buffers);
  }

  static void bind(GLuint id){
    if(id == current_framebuffer) return;
    glBindFramebuffer(GL_FRAMEBUFFER, id);
  }

  static void blit(const Framebuffer& source, GLuint destination){
    glBindFramebuffer(GL_READ_FRAMEBUFFER, source.id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination);

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glDrawBuffer(GL_BACK);

    glBlitFramebuffer(0, 0, source.width, source.height, 0, 0, window_width, window_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  }

  static void blit(const Framebuffer& source, Framebuffer& destination){
    glBindFramebuffer(GL_READ_FRAMEBUFFER, source.id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination.id);
    glBlitFramebuffer(0, 0, source.width, source.height, 0, 0, destination.width, destination.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  }

private:
  std::shared_ptr<RBO> rbo;
  std::vector<Texture> attachments;

  size_t width, height;

  void setDrawBuffers(){
    std::vector<GLenum> ids(attachments.size());
    std::iota(ids.begin(), ids.end(), GL_COLOR_ATTACHMENT0);
    glDrawBuffers(ids.size(), ids.data());
  }

  static inline GLuint current_framebuffer = -1;
};

