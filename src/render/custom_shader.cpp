#include <f2/render/custom_shader.hpp>

// Taken from https://github.com/tinyobjloader/tinyobjloader/blob/release/examples/viewer/viewer.cc
void f2::details::computeAllSmoothingNormals(tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes) {
  glm::vec3 p[3];
  for (size_t s = 0, slen = shapes.size(); s < slen; ++s) {
    const tinyobj::shape_t& shape(shapes[s]);
    size_t facecount = shape.mesh.num_face_vertices.size();
    assert(shape.mesh.smoothing_group_ids.size());

    for (size_t f = 0, flen = facecount; f < flen; ++f) {
      for (unsigned int v = 0; v < 3; ++v) {
        tinyobj::index_t idx = shape.mesh.indices[3*f + v];
        assert(idx.vertex_index != -1);
        p[v].x = attrib.vertices[3*idx.vertex_index  ];
        p[v].y = attrib.vertices[3*idx.vertex_index+1];
        p[v].z = attrib.vertices[3*idx.vertex_index+2];
      }

      // cross(p[1] - p[0], p[2] - p[0])
      glm::vec3 v10 = p[1] - p[0];
      glm::vec3 v20 = p[2] - p[0];

      glm::vec3 N = {
        v10.y * v20.z - v10.z * v20.y,
        v10.z * v20.x - v10.x * v20.z,
        v10.x * v20.y - v10.y * v20.x
      };

      // Don't normalize here.
      for (unsigned int v = 0; v < 3; ++v) {
        tinyobj::index_t idx = shape.mesh.indices[3*f + v];
        size_t index = (idx.normal_index == -1) ? idx.vertex_index : idx.normal_index;
        attrib.normals[3*index  ] += N.x;
        attrib.normals[3*index+1] += N.y;
        attrib.normals[3*index+2] += N.z;
      }
    }
  }

  assert(attrib.normals.size() % 3 == 0);
  for (size_t i = 0, nlen = attrib.normals.size() / 3; i < nlen; ++i) {
    tinyobj::real_t& nx = attrib.normals[3*i  ];
    tinyobj::real_t& ny = attrib.normals[3*i+1];
    tinyobj::real_t& nz = attrib.normals[3*i+2];
    tinyobj::real_t len = sqrtf(nx*nx + ny*ny + nz*nz);
    tinyobj::real_t scale = len == 0 ? 0 : 1 / len;
    nx *= scale;
    ny *= scale;
    nz *= scale;
  }
}
