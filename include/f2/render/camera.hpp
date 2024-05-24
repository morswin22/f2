#include <glm/glm.hpp>

namespace f2 {

  struct camera {
    glm::vec3 position;
    glm::vec3 up;

    camera(const glm::vec3 &position,
           const glm::vec3 &up = glm::vec3{0.0f, 1.0f, 0.0f});

    glm::mat4 look_at(const glm::vec3 &target) const;
  };

}
