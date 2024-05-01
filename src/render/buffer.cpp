#include <f2/render/buffer.hpp>

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
