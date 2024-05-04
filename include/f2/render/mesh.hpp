#pragma once
#include <f2/render/vao.hpp>
#include <f2/render/buffer.hpp>

namespace f2 {

  struct mesh {
    f2::vao vao;
    f2::buffer vbo;
    f2::buffer ebo;
    size_t num_indices;

    mesh(size_t num_indices);

    void render() const;
  };

}
