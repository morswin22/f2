#include <f2/render/vao.hpp>

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
