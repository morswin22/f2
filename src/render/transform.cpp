#include <f2/render/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <doctest/doctest.h>

TEST_CASE("Create transform") {
  glm::vec3 translation(0.0f, 1.0f, 2.0f);
  glm::quat rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 scale(2.0f);

  glm::mat4 desired = glm::translate(glm::mat4(1.0f), translation)
    * glm::toMat4(rotation)
    * glm::scale(glm::mat4(1.0f), scale);

  f2::transform transform{
    .translation = translation,
    .rotation = rotation,
    .scale = scale
  };
  CHECK(transform.translation == translation);
  CHECK(transform.rotation == rotation);
  CHECK(transform.scale == scale);

  glm::mat4 matrix = transform;
  CHECK(matrix == desired);

  const float* ptr = transform;
  CHECK(ptr != nullptr);
}

f2::transform::operator const float*() const {
  static glm::mat4 matrix;
  matrix = compute_matrix(translation, rotation, scale);
  return glm::value_ptr(matrix);
}

f2::transform::operator glm::mat4() const {
  return compute_matrix(translation, rotation, scale);
}

glm::mat4 f2::transform::compute_matrix(glm::vec3 translation, glm::quat rotation, glm::vec3 scale) {
  return glm::translate(glm::mat4(1.0f), translation)
    * glm::toMat4(rotation)
    * glm::scale(glm::mat4(1.0f), scale);
}

