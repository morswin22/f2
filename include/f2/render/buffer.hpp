#pragma once

#include <GL/glew.h>
#include <array>
#include <cstddef>
#include <vector>

namespace f2 {
  
  class buffer {
  public:
    buffer(GLenum target, GLenum usage = GL_STATIC_DRAW);
    buffer(const buffer&) = delete;
    buffer(buffer&& other) noexcept;
    buffer operator=(const buffer&) = delete;
    buffer& operator=(buffer&& other) noexcept;
    ~buffer();

    void bind() const;
    void unbind() const;

    void data(GLsizeiptr size, const void* data) const;

    template<typename T, std::size_t N>
    void data(const std::array<T, N>& data) {
      glBufferData(_target, sizeof(data), data.data(), _usage);
    }

    template<typename T>
    void data(const std::vector<T>& data) {
      glBufferData(_target, data.size() * sizeof(T), data.data(), _usage);
    }

    void sub_data(GLintptr offset, GLsizeiptr size, const void* data) const;

    GLuint id() const { return _id; }
    GLenum target() const { return _target; }
    GLenum usage() const { return _usage; }

  private:
    GLuint _id;
    GLenum _target;
    GLenum _usage;
  };  
  
}
