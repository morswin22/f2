#include <f2/render/camera.hpp>
#include <glm/ext.hpp>
#include <doctest/doctest.h>

TEST_CASE("Create camera") {
  f2::camera camera{ glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f) };
  CHECK(camera.position == glm::vec3(0.0f));
  CHECK(camera.up == glm::vec3(0.0f, 1.0f, 0.0f));

  auto view = camera.look_at(glm::vec3(1.0f));
  CHECK(view == glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

f2::camera::camera(const glm::vec3 &position, const glm::vec3 &up)
    : position(position), up(up) {}

glm::mat4 f2::camera::look_at(const glm::vec3 &target) const {
  return glm::lookAt(position, target, up);
}
