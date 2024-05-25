#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <cstdlib>
#include <f2/runtime/window.hpp>
#ifdef f2_enable_imgui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif
#include <stdexcept>
#include <f2/runtime/frame.hpp>
#include <doctest/doctest.h>

TEST_CASE("Create a window") {
  f2::window window;

  for (size_t i = 0; i < 2; i++) {
    f2::frame frame = window.next_frame();
    CHECK(frame.should_close == false);
    CHECK(frame.elapsed_time > 0.0);
    CHECK(frame.frame_count == i);
  }

  CHECK(glGetError() == GL_NO_ERROR);
}

std::optional<f2::initializer> f2::window::initializer{ std::nullopt };

f2::window::window() {
  if (!initializer.has_value())
    initializer.emplace();

  glfw_window = glfwCreateWindow(width, height, "f2 Window", nullptr, nullptr);
  if (!glfw_window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }
   
  glfwMakeContextCurrent(glfw_window);

  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    throw std::runtime_error("Failed to initialize GLEW");
  }

#ifdef f2_enable_imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(glfw_window, false);
  ImGui_ImplOpenGL3_Init("#version 330");
  ImGui::StyleColorsDark();
#endif

  glfwSetKeyCallback(glfw_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
#ifdef f2_enable_imgui
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (ImGui::GetIO().WantCaptureKeyboard)
      return;
#endif
  });

  glfwSetMouseButtonCallback(glfw_window, [](GLFWwindow* window, int button, int action, int mods) {
#ifdef f2_enable_imgui
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    if (ImGui::GetIO().WantCaptureMouse)
      return;
#endif
  });

  glfwSetCursorPosCallback(glfw_window, [](GLFWwindow* window, double x, double y) {
#ifdef f2_enable_imgui
    ImGui_ImplGlfw_CursorPosCallback(window, x, y);
    if (ImGui::GetIO().WantCaptureMouse)
      return;
#endif
  });

  glfwSetScrollCallback(glfw_window, [](GLFWwindow* window, double x, double y) {
#ifdef f2_enable_imgui
    ImGui_ImplGlfw_ScrollCallback(window, x, y);
    if (ImGui::GetIO().WantCaptureMouse)
      return;
#endif
  });
}

f2::window::~window() {
#ifdef f2_enable_imgui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
#endif
  glfwDestroyWindow(glfw_window);
}

f2::frame f2::window::next_frame() {
  glfwGetFramebufferSize(glfw_window, &width, &height);
  glViewport(0, 0, width, height);

  double current_time = glfwGetTime();

  f2::frame frame{
    glfwWindowShouldClose(glfw_window) != 0,
    width,
    height,
    current_time - last_time,
    frame_count++,
    *this
  };

  last_time = current_time;

  return frame;
}
