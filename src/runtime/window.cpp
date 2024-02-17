#include "GLFW/glfw3.h"
#include <f2/runtime/window.hpp>

#include <f2/runtime/frame.hpp>

f2::window::window() {
  glfw_window = glfwCreateWindow(640, 480, "Hello, World!", nullptr, nullptr);
  if (!glfw_window) {
    glfwTerminate();
    exit(-1);
  }
   
  glfwMakeContextCurrent(glfw_window);
}

f2::window::~window() {
  glfwDestroyWindow(glfw_window);
}

f2::frame f2::window::next_frame() {
  f2::frame frame(
    glfwWindowShouldClose(glfw_window) != 0,
    *this
  );

  glfwGetFramebufferSize(glfw_window, &frame.width, &frame.height);
  glViewport(0, 0, frame.width, frame.height);

  double current_time = glfwGetTime();
  frame.elapsed_time = current_time - last_time;
  last_time = current_time;

  return frame;
}

void f2::window::end_frame() {
  glfwSwapBuffers(glfw_window);
  glfwPollEvents();
}
