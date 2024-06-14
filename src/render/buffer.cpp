#include <f2/render/buffer.hpp>
#include <doctest/doctest.h>
#include <f2/runtime/window.hpp>

TEST_CASE("Create buffer") {
  f2::window window;

  f2::buffer buffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
  CHECK(buffer.id() != 0);

  SUBCASE("Use buffer") {
    buffer.bind();
    buffer.data(0, nullptr);
    buffer.sub_data(0, 0, nullptr);
    buffer.unbind();
  }

  SUBCASE("Move buffer") {
    f2::buffer other = std::move(buffer);
    buffer = std::move(other);
    CHECK(buffer.id() != 0);
    CHECK(other.id() == 0);
  }

  CHECK(glGetError() == GL_NO_ERROR);
}

f2::buffer::buffer(GLenum target, GLenum usage)
  : _target(target), _usage(usage)
{
  glGenBuffers(1, &_id);
}

f2::buffer::buffer(buffer&& other) noexcept
  : _id(other._id), _target(other._target), _usage(other._usage)
{
  other._id = 0;
}

f2::buffer& f2::buffer::operator=(buffer&& other) noexcept {
  _id = other._id;
  _target = other._target;
  _usage = other._usage;
  other._id = 0;
  return *this;
}

f2::buffer::~buffer() {
  if (_id)
    glDeleteBuffers(1, &_id);
}

void f2::buffer::bind() const {
  glBindBuffer(_target, _id);
}

void f2::buffer::unbind() const {
  glBindBuffer(_target, 0);
}

void f2::buffer::data(GLsizeiptr size, const void* data) const {
  glBufferData(_target, size, data, _usage);
}

void f2::buffer::sub_data(GLintptr offset, GLsizeiptr size, const void* data) const {
  glBufferSubData(_target, offset, size, data);
}
