#pragma once
#include "f2/runtime/initializer.hpp"
#include <GLFW/glfw3.h>
#include <optional>

namespace f2 {
  struct frame;

  class window {
    GLFWwindow* glfw_window{ nullptr };
    int width{ 640 };
    int height{ 480 };
    double last_time{ 0.0 };
    size_t frame_count{ 0u };
    static std::optional<f2::initializer> initializer;
  public:
    window();
    ~window();

    operator GLFWwindow*() { return glfw_window; }

    frame next_frame();
  };

}
