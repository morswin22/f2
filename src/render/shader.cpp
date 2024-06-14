#include <f2/render/shader.hpp>
#include <stdexcept>
#include <doctest/doctest.h>
#include <f2/runtime/window.hpp>

TEST_CASE("Create shader") {
  f2::window window;

  f2::shader shader;
  CHECK(shader.id() != 0);

  f2::shader other = std::move(shader);
  shader = std::move(other);
  CHECK(shader.id() != 0);
  CHECK(other.id() == 0);

  f2::shader_source vertex(GL_VERTEX_SHADER, R"(
    #version 330 core
    layout(location = 0) in vec3 position;
    void main() {
      gl_Position = vec4(position, 1.0);
    }
  )");

  f2::shader_source fragment(GL_FRAGMENT_SHADER, R"(
    #version 330 core
    out vec4 color;
    void main() {
      color = vec4(1.0, 0.0, 0.0, 1.0);
    }
  )");

  f2::shader_source other_source = std::move(vertex);
  vertex = std::move(other_source);

  try {
    f2::shader_source bad_source(GL_FRAGMENT_SHADER, R"(bad code)");
  } catch (const std::runtime_error& e) {
    CHECK(e.what() != nullptr);
  }

  shader.source(std::move(vertex));
  shader.source(std::move(fragment));
  shader.compile();

  shader.bind();
  CHECK(shader.attribute("position") == 0);
  CHECK(shader.attribute("position") == 0);
  CHECK(shader.uniform("color") == -1);
  CHECK(shader.uniform("color") == -1);
  shader.unbind();

  CHECK(glGetError() == GL_NO_ERROR);
}

f2::shader_source::shader_source(GLenum type, const char* source) : _type(type) {
  _id = glCreateShader(_type);
  glShaderSource(_id, 1, &source, nullptr);
  
  GLint status;
  glCompileShader(_id);
  glGetShaderiv(_id, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint length;
    glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length);
    std::string log(length, '\0');
    glGetShaderInfoLog(_id, length, nullptr, log.data());
    throw std::runtime_error(log);
  }
}

f2::shader_source::shader_source(shader_source&& other) noexcept : _id(other._id), _type(other._type) {
  other._id = 0;
}

f2::shader_source& f2::shader_source::operator=(shader_source&& other) noexcept {
  _id = other._id;
  _type = other._type;
  other._id = 0;
  return *this;
}

f2::shader_source::~shader_source() {
  if (_id)
    glDeleteShader(_id);
}

void f2::shader_source::attach(GLuint program) const {
  glAttachShader(program, _id);
}

void f2::shader_source::detach(GLuint program) const {
  glDetachShader(program, _id);
}

f2::shader::shader() {
  _id = glCreateProgram();
}

f2::shader::~shader() {
  if (_id)
    glDeleteProgram(_id);
}

f2::shader::shader(shader&& other) noexcept
  : _id(other._id), shaders(std::move(other.shaders)), attributes(other.attributes), uniforms(other.uniforms)
{
  other._id = 0;
  other.shaders.clear();
  other.attributes.clear();
  other.uniforms.clear();
}

f2::shader& f2::shader::operator=(shader&& other) noexcept {
  _id = other._id;
  shaders = std::move(other.shaders);
  attributes = other.attributes;
  uniforms = other.uniforms;
  other._id = 0;
  other.shaders.clear();
  other.attributes.clear();
  other.uniforms.clear();
  return *this;
}

void f2::shader::bind() const {
  glUseProgram(_id);
}

void f2::shader::unbind() const {
  glUseProgram(0);
}

GLuint f2::shader::attribute(std::string_view attribute) {
  auto it = attributes.find(attribute.data());
  if (it != attributes.end())
    return it->second;
  GLuint id = glGetAttribLocation(_id, attribute.data());
  attributes[attribute.data()] = id;
  return id;
}

GLuint f2::shader::uniform(std::string_view uniform) {
  auto it = uniforms.find(uniform.data());
  if (it != uniforms.end())
    return it->second;
  GLuint id = glGetUniformLocation(_id, uniform.data());
  uniforms[uniform.data()] = id;
  return id;
}

void f2::shader::source(shader_source&& source) {
  source.attach(_id);
  shaders.push_back(std::move(source));
}

void f2::shader::compile() {
  GLint status;
  glLinkProgram(_id);
  glGetProgramiv(_id, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint length;
    glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
    std::string log(length, '\0');
    glGetProgramInfoLog(_id, length, nullptr, log.data());
    throw std::runtime_error(log);
  }

  shaders.clear();
}
