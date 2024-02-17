#pragma once
#include <GLFW/glfw3.h>

#include <cstdlib>

namespace f2 {
  struct frame;

  class window {
    GLFWwindow* glfw_window{ nullptr };
    double last_time{ 0.0 };
  public:
    window();
    ~window();

    frame next_frame();
    void end_frame();
  };

}
