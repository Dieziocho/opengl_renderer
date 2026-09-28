#include "camera.h"
#include "cursor.h"
#include <glm/vec2.hpp>

namespace Cursor {
  glm::vec2 position(0.0f, 0.0f);

  void moveCallback(GLFWwindow*, double x_position, double y_position){
    glm::vec2 new_position(x_position, y_position);
    glm::vec2 offset = new_position - position;
    position = new_position;

    offset = {-offset.y, offset.x};

    Camera::rotateBy(offset);
  }
}
