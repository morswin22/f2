#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <f2/runtime/window.hpp>
#include <f2/runtime/frame.hpp>
#include <f2/asset/image.hpp>
#include <f2/asset/asset.hpp>
#include <f2_assets.hpp>
#include <cstdio>

int main(void) {
  f2::window window;
  glfwSetWindowTitle(window, "Hello, World!");

  f2::image brick_texture = f2::asset<"bricks_texture.jpg">();
  printf("w %d, h %d, c %d\n", brick_texture.width, brick_texture.height, brick_texture.channels);
  // w 256, h 256, c 3 

  f2::image not_exist = f2::asset<"bg.png">();
  printf("w %d, h %d, c %d\n", not_exist.width, not_exist.height, not_exist.channels);
  // w 0, h 0, c 0
  // in the future default to 2x2 pink and black board and f2::warn()

  // maybe not existient f2::asset should be a compile error
  // and 2x2 pink/black board with f2::warn() should apply only to runtime f2::image("not_exist.png");

  glEnable(GL_TEXTURE_2D);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0,
      GL_RGB, brick_texture.width, brick_texture.height, 0,
      GL_RGB, GL_UNSIGNED_BYTE, brick_texture.pixels);

  glm::vec2 pos{ 0, 0 };
  glm::vec2 delta{ 1, 1 };
  float size{ 45 };
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
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(pos.x, pos.y);

      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(pos.x + size, pos.y);

      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(pos.x + size, pos.y + size);

      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(pos.x, pos.y + size);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glFlush();

    // UI
    ImGui::Begin("Info");
    ImGui::TextWrapped("elapsed time %f", frame.elapsed_time);
    ImGui::End();
  }

  glDeleteTextures(1, &texture);

  return 0;
}
