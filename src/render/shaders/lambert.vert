#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

out VS_out
{
  vec3 frag_normal;
  vec3 frag_pos;
} vs_out;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
  mat3 normal_matrix   = mat3(transpose(inverse(model)));
  vec4 _position       = vec4(position, 1.f);

  vs_out.frag_pos      = (model * _position).xyz;
  vs_out.frag_normal   = normal_matrix * normal;

  gl_Position = projection * view * model * _position;
}
