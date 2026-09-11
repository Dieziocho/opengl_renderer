#include "base_buffer.h"

struct UboDeleter {
  void operator()(GLuint id) const {
    glDeleteBuffers(1, &id);
  }
};

class UBO : public BaseBuffer<UboDeleter> {
public:
  UBO() = default;

  void reserve(size_t size){
    buffer_size = size;
    glGenBuffers(1, &id);
    glBindBuffer(GL_UNIFORM_BUFFER, id);
    glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_DYNAMIC_DRAW);
  }

  template<typename T>
  void update(const T& data){
    glBindBuffer(GL_UNIFORM_BUFFER, id);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer_size, &data);
  }

  void bind(unsigned ubo_index) const {
    glBindBufferBase(GL_UNIFORM_BUFFER, ubo_index, id);
  }

private:
  size_t buffer_size = 0;
};

