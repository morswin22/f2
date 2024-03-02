#include <f2/runtime/frame.hpp>
#ifdef f2_enable_imgui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

f2::frame::frame(bool _should_close, int _width, int _height, double _elapsed_time, size_t _frame_count, f2::window& _window)
  : should_close(_should_close), width(_width), height(_height), elapsed_time(_elapsed_time), frame_count(_frame_count), window(_window) {
#ifdef f2_enable_imgui
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
#endif
}

f2::frame::~frame() {
#ifdef f2_enable_imgui
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

  glfwSwapBuffers(window);
  glfwPollEvents();
}
