#include <f2/render/mesh.hpp>

f2::mesh::mesh(size_t num_indices)
  : vao(), vbo(GL_ARRAY_BUFFER), ebo(GL_ELEMENT_ARRAY_BUFFER), num_indices(num_indices)
{}

void f2::mesh::render() const {
  glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
}
