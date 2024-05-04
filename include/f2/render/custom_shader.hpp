#pragma once
#include <f2/util/scoped_bind.hpp>
#include <f2/util/visit.hpp>
#include <f2/render/shader.hpp>
#include <f2/render/mesh.hpp>
#include <f2/asset/asset.hpp>
#include <f2/f2_assets.hpp>
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>

namespace f2 {

  namespace details {    
    void computeAllSmoothingNormals(tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes);
  }

  template<class V, class U, string_literal ... Ss>
  class custom_shader : public shader {
  public:
    custom_shader() {
      scoped_bind sb{ *this };

      (source(asset<Ss>()), ...);
      compile();
    }

    using vertex = V;
    using uniforms = U;

    mesh get_mesh(const vertex* data, size_t num_vertices, const GLuint* indices, size_t num_indices) {
      mesh m{ num_indices };

      scoped_bind sb{ *this, m.vao, m.vbo, m.ebo };
      
      m.vbo.data(num_vertices * sizeof(vertex), data);
     
      visit_each<vertex>([&](auto pair) {
        m.vao.attribute(attribute(pair.first), pair.second);
      });

      m.ebo.data(num_indices * sizeof(GLuint), indices);

      return m;
    }

    template<size_t N, size_t M>
    mesh get_mesh(const std::array<vertex, N>& data, const std::array<GLuint, M>& indices) {
      return get_mesh(data.data(), N, indices.data(), M);
    }

    mesh get_mesh(const std::vector<vertex>& data, const std::vector<GLuint>& indices) {
      return get_mesh(data.data(), data.size(), indices.data(), indices.size());
    }

    mesh get_mesh(const tinyobj::ObjReader& reader, const tinyobj::shape_t& model) {
      std::vector<vertex> vertices;
      std::vector<GLuint> indices;

      auto attributes = reader.GetAttrib();

      // TODO: only do this if vertex::normal is present
      if (attributes.normals.size() == 0) {
        attributes.normals.resize(attributes.vertices.size(), 0.0f);
        details::computeAllSmoothingNormals(attributes, reader.GetShapes());
      }

      size_t index_offset = 0;
      for (auto num_vertices : model.mesh.num_face_vertices) {
        if (num_vertices != 3) {
          throw std::runtime_error("Only triangles are supported");
        }
        for (size_t i = 0; i < num_vertices; ++i) {
          tinyobj::index_t index = model.mesh.indices[index_offset + i];
          vertex vertex;
          visit_each(vertex, [&](auto pair) {
            map_member(pair.first, pair.second, index, attributes);
          });
          vertices.push_back(vertex);
          indices.push_back(index_offset + i); // TODO: this makes it so we have equal number of indices as vertices, so is bad?
        }
        index_offset += num_vertices;
      }

      return get_mesh(vertices, indices);
    }

    mesh get_mesh(const tinyobj::ObjReader& reader) {
      return get_mesh(reader, reader.GetShapes()[0]);
    }

  private:
    template<class T>
    void map_member(std::string_view name, T& member, tinyobj::index_t index_t, const tinyobj::attrib_t& attributes) {
      // TODO: call custom map if provided
    }

    void map_member(std::string_view name, glm::vec3& member, tinyobj::index_t index, const tinyobj::attrib_t& attributes) {
      if (name == "position") {
        size_t idx = 3*index.vertex_index;
        member = glm::vec3{ attributes.vertices.at(idx + 0), attributes.vertices.at(idx + 1), attributes.vertices.at(idx + 2) };
      }
      else if (name == "normal") {
        size_t idx = (index.normal_index == -1) ? index.vertex_index : index.normal_index;
        idx *= 3;
        member = glm::vec3{ attributes.normals.at(idx + 0), attributes.normals.at(idx + 1), attributes.normals.at(idx + 2) };
      }
    }

    void map_member(std::string_view name, glm::vec2& member, tinyobj::index_t index, const tinyobj::attrib_t& attributes) {
      if (name == "uv") {
        if (index.texcoord_index == -1)
          return;
        size_t idx = 2*index.texcoord_index;
        member = glm::vec2{ attributes.texcoords.at(idx + 0), attributes.texcoords.at(idx + 1) };
      }
    }
  };

  // Lambert shader

  namespace details {
    F2_VISITABLE_STRUCT(lambert_vertex,
      (glm::vec3) position,
      (glm::vec3) normal,
      (glm::vec2) uv
    );

    struct lambert_uniforms {
      // Vertex shader
      glm::mat4 projection;
      glm::mat4 view;
      glm::mat4 model;
      // Fragment shader
      struct LightSource {
        glm::vec3 position;
        glm::vec3 intensity;
      } light;
      struct Material {
        glm::vec4 color;
        glm::vec3 ambient;
      } material;
    };
  }

  using lambert_shader = custom_shader<details::lambert_vertex, details::lambert_uniforms, "lambert.vert", "lambert.frag">;

}
