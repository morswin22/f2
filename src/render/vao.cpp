#include <f2/render/vao.hpp>
#include <f2/runtime/window.hpp>
#include <doctest/doctest.h>

TEST_CASE("Create VAO") {
  f2::window window;

  f2::vao vao;
  CHECK(vao.id() != 0);

  vao.bind();
  vao.attribute(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  vao.unbind();

  CHECK(glGetError() == GL_NO_ERROR);
}

f2::vao::vao() {
  glGenVertexArrays(1, &_id);
}

f2::vao::~vao() {
  if (_id)
    glDeleteVertexArrays(1, &_id);
}

void f2::vao::bind() const {
  glBindVertexArray(_id);
}

void f2::vao::unbind() const {
  glBindVertexArray(0);
}

void f2::vao::attribute(GLuint index, GLint dimensions, GLenum type, GLenum normalized, GLsizei stride, const void* offset) {
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index, dimensions, type, normalized, stride, offset);
}
