#include "camera.h"
#include "ubo_class.h"
#include "config.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

struct CameraParameters {
  glm::mat4 projection_view;
  glm::vec3 camera_position;
};

UBO parameters_ubo;

namespace Camera {
  glm::vec3 target_position = {0,0,0};
  glm::vec2 angle = {0, glm::radians(90.0f)};
  float distance = 0;
  bool changed = true;

  glm::vec3 front, right, up;

  void init(){
    //Create an uniform buffer, to pass matrices easier across shaders
    parameters_ubo.reserve(sizeof(CameraParameters));
    parameters_ubo.bind(0);
    rotateBy({0,0});
    update();
  }

  void update(){
    if(!changed) return;
    changed = false;

    CameraParameters parameters;
    parameters.projection_view = getProjectionMatrix() * getViewMatrix();
    parameters.camera_position = getPosition();

    parameters_ubo.update(parameters);
  }

  void moveBy(glm::vec3 offset){
    target_position += offset;
    changed = true;
  }

  void rotateBy(glm::vec2 offset){
    angle += offset * camera_sensitivity;

    //Keep yaw in the -180 +180 degree range
    float yaw_range = glm::radians(180.0f);
    if(angle.y <= -yaw_range) angle.y += yaw_range * 2;
    angle.y = fmod(angle.y + yaw_range, yaw_range * 2) - yaw_range;

    //Clamp pitch
    float pitch_range = glm::radians(100.0f);
    angle.x = glm::clamp(angle.x, -pitch_range, pitch_range);

    //Lock front vector to y=0
    front.x = cos(angle.y);
    front.y = 0;
    front.z = sin(angle.y);

    //Right vector to calculate pitch and movement
    right = glm::normalize(
      glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))
      );

    //Up vector is the world up rotated by pitch in the right vector
    up = glm::rotate(world_up, angle.x, right);

    changed = true;
  }

  std::array<glm::vec3, 3> getDirections(){
    return {front, right, up};
  }

  glm::vec3 getDirection(){
    glm::vec3 direction = glm::rotate(up, glm::radians(-90.0f), right);
    return direction;
  }

  glm::vec3 getPosition(){
    glm::vec3 direction = getDirection();
    glm::vec3 camera_position = target_position - direction * distance;
    return camera_position;
  }

  glm::mat4 getViewMatrix(){
    glm::vec3 direction = getDirection();
    glm::vec3 camera_position = getPosition();
    glm::vec3 target = target_position + (direction * camera_target_offset);

    return glm::lookAt(
      camera_position,
      target,
      up
      );
  }

  glm::mat4 getProjectionMatrix(){
    return glm::perspective(glm::radians(fov), (float)window_width/(float)window_height, camera_near, camera_far);
  }
}
