#pragma once
#include "f2/runtime/initializer.hpp"
#include <GLFW/glfw3.h>
#include <optional>

namespace f2 {
  struct frame;

  class window {
    GLFWwindow* glfw_window{ nullptr };
    double last_time{ 0.0 };
    static std::optional<f2::initializer> initializer;
  public:
    window();
    ~window();

    frame next_frame();
    void end_frame();
  };

}
