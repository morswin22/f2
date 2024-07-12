    operator f2::shader_source() const {
      char source[size + 1];
      strncpy(source, data, size);
      source[size] = '\0';
      return f2::shader_source(GL_{{shader_type}}_SHADER, source);
    }
