#include <f2/render/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

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

