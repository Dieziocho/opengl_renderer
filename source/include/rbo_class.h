#pragma once
#include "base_buffer.h"

struct RboDeleter {
  void operator()(GLuint id) const {
    glDeleteRenderbuffers(1, &id);
  }
};

class RBO : public BaseBuffer<RboDeleter> {
public:
  RBO(size_t sample_count, unsigned width, unsigned height){
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    if(sample_count > 1)
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, sample_count, GL_DEPTH24_STENCIL8, width, height);
    else
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  }

  void attach(){
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);
  }
};

