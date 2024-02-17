#include "GLFW/glfw3.h"
#include <cstdlib>
#include <f2/runtime/initializer.hpp>

f2::initializer::initializer() {
  if (!glfwInit())
    exit(-1);
}

f2::initializer::~initializer() {
  glfwTerminate();
}
