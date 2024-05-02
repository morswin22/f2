#pragma once

#include <GL/glew.h>
#include <glm/detail/qualifier.hpp>
#include <f2/util/type_to_enum.hpp>

namespace f2 {
  
  class vao {
  public:
    vao();
    vao(const vao&) = delete;
    vao(vao&& other) noexcept;
    vao operator=(const vao&) = delete;
    vao& operator=(vao&& other) noexcept;
    ~vao();

    void bind() const;
    void unbind() const;

    void attribute(GLuint index, GLint dimensions, GLenum type, GLenum normalized = GL_FALSE, GLsizei stride = 0, const void* offset = nullptr);

    template<typename C, typename T>
    void attribute(GLuint index, T C::* member, GLint dimensions, GLenum type, GLenum normalized = GL_FALSE) {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(index, dimensions, type, normalized, sizeof(C), (void*)&(reinterpret_cast<C const volatile*>(NULL)->*member));
    }

    template<typename C, GLint D, typename T, glm::qualifier Q>
    void attribute(GLuint index, glm::vec<D, T, Q> C::* member, GLenum normalized = GL_FALSE) {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(index, D, type_to_enum<T>(), normalized, sizeof(C), (void*)&(reinterpret_cast<C const volatile*>(NULL)->*member));
    }

    // TODO: store buffers inside vao

    GLuint id() const { return _id; }

  private:
    GLuint _id;
  };

}
