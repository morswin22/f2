#pragma once

#include <GL/glew.h>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace f2 {

  class shader_source {
  public:
    shader_source(GLenum type, const char* source);
    shader_source(const shader_source&) = delete;
    shader_source(shader_source&& other) noexcept;
    shader_source& operator=(const shader_source&) = delete;
    shader_source& operator=(shader_source&& other) noexcept;
    ~shader_source();

    void attach(GLuint program) const;
    void detach(GLuint program) const;

    GLuint id() const { return _id; }
    GLenum type() const { return _type; }

  private:
    GLuint _id;
    GLenum _type;
  };

  class shader {
  public:
    shader();
    shader(const shader&) = delete;
    shader(shader&& other) noexcept;
    shader operator=(const shader&) = delete;
    shader& operator=(shader&& other) noexcept;
    ~shader();

    void bind() const;
    void unbind() const;

    void source(shader_source&& source);
    void compile();

    GLuint attribute(std::string_view attribute);
    GLuint uniform(std::string_view uniform);

    GLuint id() const { return _id; }

  private:
    GLuint _id;
    std::vector<shader_source> shaders;
    std::map<std::string, GLuint> attributes;
    std::map<std::string, GLuint> uniforms;
  };

}
