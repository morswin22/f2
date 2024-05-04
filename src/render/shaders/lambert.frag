#version 330

layout(location = 0) out vec4 out_color;

in VS_out
{
  vec3 frag_normal;
  vec3 frag_pos;
} fs_in;

struct Material
{
  vec4 color;
  vec3 ambient;
};

struct LightSource
{
  vec3 position;
  vec3 intensity;
};

uniform Material material;
uniform LightSource light;

void main()
{
  vec3 ambient = normalize(material.ambient);

  vec3 norm        = normalize(fs_in.frag_normal);
  vec3 pos         = fs_in.frag_pos;
  vec3 light_dir   = normalize(light.position - pos);

  float lambert       = max(dot(norm, light_dir), 0.0);
  vec3 illumination   = (lambert * light.intensity) + ambient;

  out_color = vec4(illumination * material.color.xyz, 1.f);
}
