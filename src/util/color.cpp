#include <f2/util/color.hpp>

glm::vec4 f2::color(uint32_t color) {
  return {
    (float)((color >> 24) & 0xFF) / 255.0f,
    (float)((color >> 16) & 0xFF) / 255.0f,
    (float)((color >> 8) & 0xFF) / 255.0f,
    (float)(color & 0xFF) / 255.0f
  };
}
