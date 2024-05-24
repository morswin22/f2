#include <f2/render/camera.hpp>
#include <glm/ext.hpp>

f2::camera::camera(const glm::vec3 &position, const glm::vec3 &up)
    : position(position), up(up) {}

glm::mat4 f2::camera::look_at(const glm::vec3 &target) const {
  return glm::lookAt(position, target, up);
}
