#include <GL/gl.h>
#include <glm/glm.hpp>
#include <f2/runtime/window.hpp>
#include <f2/runtime/frame.hpp>

int main(void) {
  f2::window window;

  glm::vec2 pos{ 0, 0 };
  glm::vec2 delta{ 1, 1 };
  float size{ 25 };
  float speed{ 150 };

  while (f2::frame frame = window.next_frame()) {
    // Update
    pos += delta * speed * (float)frame.elapsed_time;
    if (pos.x + size > frame.width) {
      pos.x = frame.width - size;
      delta.x = -1;
    }
    else if (pos.x < 0) {
      pos.x = 0;
      delta.x = 1;
    }
    if (pos.y + size > frame.height) {
      pos.y = frame.height - size;
      delta.y = -1;
    }
    else if (pos.y < 0) {
      pos.y = 0;
      delta.y = 1;
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
    glRectf(pos.x, pos.y, pos.x + size, pos.y + size);

    glFlush();
  }

  return 0;
}
