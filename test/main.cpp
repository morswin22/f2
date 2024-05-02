#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <f2/runtime/window.hpp>
#include <f2/runtime/frame.hpp>
#include <f2/asset/image.hpp>
#include <f2/asset/asset.hpp>
#include <f2_assets.hpp>
#include <f2/render/transform.hpp>
#include <f2/render/buffer.hpp>
#include <f2/render/vao.hpp>
#include <f2/util/scoped_bind.hpp>
#include <f2/render/shader.hpp>
#include <cstdio>
#include <stdexcept>

int main(void) try {
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
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0,
      GL_RGB, brick_texture.width, brick_texture.height, 0,
      GL_RGB, GL_UNSIGNED_BYTE, brick_texture.pixels);

  f2::shader shader;
  f2::vao vao;
  f2::buffer vbo(GL_ARRAY_BUFFER);
  f2::buffer ebo(GL_ELEMENT_ARRAY_BUFFER);

  {
    f2::scoped_bind sb{ shader, vao, vbo, ebo };

    shader.source(f2::asset<"texture.vert">());
    shader.source(f2::asset<"texture.frag">());
    shader.compile();
  
    struct Vertex {
      glm::vec3 position;
      glm::vec2 texUV;
    };

    vbo.data(std::array{
      Vertex{ glm::vec3{ -0.5f,  0.5f, 0.0f }, glm::vec2{ 0.0f, 1.0f } },
      Vertex{ glm::vec3{  0.5f,  0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f } },
      Vertex{ glm::vec3{  0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 0.0f } },
      Vertex{ glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 0.0f, 0.0f } }
    });
    vao.attribute(shader.attribute("position"), &Vertex::position);
    vao.attribute(shader.attribute("texUV"), &Vertex::texUV);
 
    ebo.data(std::array{
      0, 1, 3,
      1, 2, 3
    });
  };

  glm::mat4 view = glm::mat4(1.0f);

  f2::transform transform{
    .scale = glm::vec3(45.0f)
  };
  glm::vec3 delta{ 1, 1, 0 };
  float speed{ 150 };

  while (f2::frame frame = window.next_frame()) {
    // Update
    transform.translation += delta * speed * (float)frame.elapsed_time;
    if (transform.translation.x + transform.scale.x*0.5f > frame.width) {
      transform.translation.x = frame.width - transform.scale.x*0.5f;
      delta.x = -1;
    }
    else if (transform.translation.x - transform.scale.x*0.5f < 0) {
      transform.translation.x = transform.scale.x*0.5f;
      delta.x = 1;
    }
    if (transform.translation.y + transform.scale.y*0.5f > frame.height) {
      transform.translation.y = frame.height - transform.scale.y*0.5f;
      delta.y = -1;
    }
    else if (transform.translation.y - transform.scale.y*0.5f < 0) {
      transform.translation.y = transform.scale.y*0.5f;
      delta.y = 1;
    }

    transform.rotation = glm::quat(glm::vec3(0.0f, 0.0f, (float)(frame.frame_count%360)*(float)M_PI/180.0f));

    // Draw
    glClearColor(0.128f, 0.128f, 0.128f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  
    glm::mat4 projection = glm::ortho(0.0f, (float)frame.width, 0.0f, (float)frame.height, -1.0f, 1.0f);

    shader.bind();
    glUniformMatrix4fv(shader.uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(shader.uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.uniform("model"), 1, GL_FALSE, transform);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(shader.uniform("tex0"), 0);

    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    vao.unbind();

    glBindTexture(GL_TEXTURE_2D, 0);

    shader.unbind();

    // UI
    ImGui::Begin("Info");
    ImGui::TextWrapped("elapsed time %f", frame.elapsed_time);
    ImGui::End();
  }

  glDeleteTextures(1, &texture);

  return 0;
} catch (const std::runtime_error& e) {
  fprintf(stderr, "Runtime Error: %s\n", e.what());
  return 1;
} catch (const std::exception& e) {
  fprintf(stderr, "Error: %s\n", e.what());
  return 1;
} catch (...) {
  fprintf(stderr, "Unknown Error\n");
  return 1;
}
