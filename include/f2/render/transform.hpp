#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace f2 {
  struct transform {
    glm::vec3 translation;
    glm::quat rotation;
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

    operator const float*() const;

    operator glm::mat4() const;

    static glm::mat4 compute_matrix(glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
  };
}
