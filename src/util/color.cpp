#include <f2/util/color.hpp>
#include <doctest/doctest.h>

TEST_CASE("Convert color") {
  auto color = f2::color(0xab78012F);
  CHECK(color.r == 0xab / 255.0f);
  CHECK(color.g == 0x78 / 255.0f);
  CHECK(color.b == 0x01 / 255.0f);
  CHECK(color.a == 0x2f / 255.0f);
}

glm::vec4 f2::color(uint32_t color) {
  return {
    (float)((color >> 24) & 0xFF) / 255.0f,
    (float)((color >> 16) & 0xFF) / 255.0f,
    (float)((color >> 8) & 0xFF) / 255.0f,
    (float)(color & 0xFF) / 255.0f
  };
}
