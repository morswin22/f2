#include <GL/gl.h>
#include <cstdlib>
#include <f2/runtime/window.hpp>
#include <f2/runtime/frame.hpp>

struct libinit {
  libinit() {
    if (!glfwInit())
      exit(-1);    
  }
  ~libinit() {
    glfwTerminate();
  }
};



int main(void) {
  libinit initializer;

  {
  f2::window window;

  float x{ 320 }, y{ 240 };
  float size{ 10 };
  float dx{ 1 }, dy{ 1 };
  float speed{ 50 };

  while (f2::frame frame = window.next_frame()) {
    // Update
    x += dx * speed * frame.elapsed_time;
    if (x + size > frame.width) {
      x = frame.width - size;
      dx = -1;
    }
    else if (x < 0) {
      x = 0;
      dx = 1;
    }

    y += dy * speed * frame.elapsed_time;
    if (y + size > frame.height) {
      y = frame.height - size;
      dy = -1;
    }
    else if (y < 0) {
      y = 0;
      dy = 1;
    }

    // Draw
    glClearColor(0.128f, 0.128f, 0.128f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double)frame.width, 0, (double)frame.height, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.862f, 0.376f, 0.407f);
    glRectf(x, y, x + size, y + size);

    glFlush();
  }
  }

  return 0;
}
