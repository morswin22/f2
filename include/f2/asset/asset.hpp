#pragma once
#include <f2/util/string_literal.hpp>
#include <f2/asset/image.hpp>
#include <f2/render/shader.hpp>
#include <fstream>

namespace f2 {
  template <string_literal S>
  struct asset {
    operator image() const {
      return image(S.value); // may not be an image
    }
    operator shader_source() const {
      std::ifstream file(S.value);
      if (!file)
        throw std::runtime_error("could not open file");
      std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
      GLenum type = GL_VERTEX_SHADER;
      if (S.ends_with(".frag"))
        type = GL_FRAGMENT_SHADER;
      else if (S.ends_with(".geom"))
        type = GL_GEOMETRY_SHADER;
      return shader_source(type, source.c_str()); // may not be a shader source
    }
    // other operators to turn into any asset (image, shader, etc)
  };
}
