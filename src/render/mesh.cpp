#include <f2/render/mesh.hpp>
#include <doctest/doctest.h>
#include <f2/runtime/window.hpp>

TEST_CASE("Create mesh") {
  f2::window window;

  f2::mesh mesh(6);
  CHECK(mesh.num_indices == 6);
}

f2::mesh::mesh(size_t num_indices)
  : vao(), vbo(GL_ARRAY_BUFFER), ebo(GL_ELEMENT_ARRAY_BUFFER), num_indices(num_indices)
{}

void f2::mesh::render() const {
  glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
}
