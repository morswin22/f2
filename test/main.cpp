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
#include <f2/render/transform.hpp>
#include <f2/render/buffer.hpp>
#include <f2/render/vao.hpp>
#include <f2/render/custom_shader.hpp>
#include <f2/util/scoped_bind.hpp>
#include <f2/render/shader.hpp>
#include <f2/render/camera.hpp>
#include <main/f2_assets.hpp>
#include <cstdio>
#include <stdexcept>

F2_VISITABLE_STRUCT(Vertex,
  (glm::vec3) position,
  (glm::vec2) texUV
);

struct Uniforms {
  glm::mat4 projection;
  glm::mat4 view;
  glm::mat4 model;
  GLint tex0;
};

int main(void) try {
  f2::window window;
  glfwSetWindowTitle(window, "Hello, World!");

  f2::lambert_shader lambert;
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  f2::mesh bunny = lambert.get_mesh(f2::asset<"bunny.obj">());
  f2::transform bunny_transform = {
    .translation = glm::vec3(0.0f, 0.0f, 0.0f),
    .scale = glm::vec3(1.0f),
  };

  f2::lambert_shader::uniforms::LightSource light = {
    .position = glm::vec3(0.0f, 1.0f, 0.0f),
    .intensity = glm::vec3(1.0f)
  };

  f2::lambert_shader::uniforms::Material material = {
    .color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
    .ambient = glm::vec3(1.0f)
  };

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

  f2::custom_shader<Vertex, Uniforms, "texture.vert", "texture.frag"> shader;

  f2::mesh quad = shader.get_mesh(std::array{
    Vertex{ glm::vec3{ -0.5f,  0.5f, 0.0f }, glm::vec2{ 0.0f, 1.0f } },
    Vertex{ glm::vec3{  0.5f,  0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f } },
    Vertex{ glm::vec3{  0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 0.0f } },
    Vertex{ glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 0.0f, 0.0f } }
  }, std::array{
    0u, 1u, 3u,
    1u, 2u, 3u
  });

  f2::transform transform{
    .translation = glm::vec3(0.0f, 0.2f, -1.0f),
    .scale = glm::vec3(0.1f)
  };
  glm::vec3 delta{ 1, 1, 0 };
  float speed{ 0.1f };

  glm::vec3 bounce_bounds[2]{
    glm::vec3(-0.5f, -0.5f, 0.0f),
    glm::vec3( 0.5f,  0.5f, 0.0f)
  };

  f2::camera camera{ glm::vec3{ 0.0f, 0.3f, 0.4f} };

  while (f2::frame frame = window.next_frame()) {
    // Update
    transform.translation += delta * speed * (float)frame.elapsed_time;
    if (transform.translation.x + transform.scale.x*0.5f > bounce_bounds[1].x) {
      transform.translation.x = bounce_bounds[1].x - transform.scale.x*0.5f;
      delta.x = -1;
    }
    else if (transform.translation.x - transform.scale.x*0.5f < bounce_bounds[0].x) {
      transform.translation.x = bounce_bounds[0].x + transform.scale.x*0.5f;
      delta.x = 1;
    }
    if (transform.translation.y + transform.scale.y*0.5f > bounce_bounds[1].y) {
      transform.translation.y = bounce_bounds[1].y - transform.scale.y*0.5f;
      delta.y = -1;
    }
    else if (transform.translation.y - transform.scale.y*0.5f < bounce_bounds[0].y) {
      transform.translation.y = bounce_bounds[0].y + transform.scale.y*0.5f;
      delta.y = 1;
    }

    const float alpha = (float)(frame.frame_count % 360) * (float)M_PI/180.0f;
    transform.rotation = glm::quat(glm::vec3(0.0f, 0.0f, alpha));
    bunny_transform.rotation = glm::quat(glm::vec3(0.0f, alpha, 0.0f));
 
    const glm::mat4 view = camera.look_at(bunny_transform.translation + glm::vec3(0.0f, 0.1f, 0.0f));
    const glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)frame.width/(float)frame.height, 0.1f, 1000.0f);

    // Draw
    glClearColor(0.128f, 0.128f, 0.128f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    {
      f2::scoped_bind fsb{ shader, quad.vao };

      glUniformMatrix4fv(shader.uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
      glUniformMatrix4fv(shader.uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(shader.uniform("model"), 1, GL_FALSE, transform);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);
      glUniform1i(shader.uniform("tex0"), 0);

      quad.render();

      glBindTexture(GL_TEXTURE_2D, 0);
    }

    {
      f2::scoped_bind fsb{ lambert, bunny.vao };

      glUniformMatrix4fv(lambert.uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
      glUniformMatrix4fv(lambert.uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(lambert.uniform("model"), 1, GL_FALSE, bunny_transform);
      
      glUniform3fv(lambert.uniform("light.position"), 1, glm::value_ptr(light.position));
      glUniform3fv(lambert.uniform("light.intensity"), 1, glm::value_ptr(light.intensity));
      glUniform4fv(lambert.uniform("material.color"), 1, glm::value_ptr(material.color));
      glUniform3fv(lambert.uniform("material.ambient"), 1, glm::value_ptr(material.ambient));

      bunny.render();
    }

    // UI
    ImGui::Begin("Info");
    ImGui::TextWrapped("elapsed time %f", frame.elapsed_time);
    ImGui::SliderFloat3("camera position", glm::value_ptr(camera.position), -1.0f, 1.0f);
    ImGui::SliderFloat3("light position", glm::value_ptr(light.position), -1.0f, 1.0f);
    ImGui::ColorPicker3("bunny color", glm::value_ptr(material.color));
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
