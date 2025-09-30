#version 330 core

in VertexData
{
    vec3 position_view;
    vec3 normal_view;
    vec3 tangent_view;
    vec3 bitangent_view;
    vec2 texCoord;
    vec3 light_view;
    vec3 light_tbn;
    vec3 cam_tbn;
} vsData;

out vec4 color;

uniform sampler2D myTextureSampler;
uniform sampler2D aoTextureSampler;    // Carte d'occlusion ambiante
uniform sampler2D normalTextureSampler; // Normal map

// n is the normal (should be normalized), 
// light_dir the direction TO the light (should be normalized),
// cam_dir the direction TO the camera (should be normalized),
// color_mat the color of the object for the current fragment,
// color_light the color of the light 
vec3 phong(vec3 n, vec3 light_dir, vec3 cam_dir, vec3 color_mat, vec3 color_light, float ka, float kd, float ks, int shininess)
{
  n = normalize(n);  // IS IT NEEDED?

  float ambiant = ka;
  float diffuse = kd*clamp(dot(n, light_dir), 0.0, 1.0);
  float specular = ks*pow(clamp(dot(cam_dir, reflect(-light_dir, n)), 0.0, 1.0), shininess);

  return (ambiant + diffuse) * color_mat + specular * color_light;
}

void main()
{
  vec3 normal_tbn = (texture(normalTextureSampler, vsData.texCoord).xyz * 2) - 1;


  float ka = 0.2; // coefficient ambiant
  float kd = 0.8; // coefficient diffuse
  float ks = 0.3; // coefficient speculaire
  int shininess = 256;

  vec3 color_mat = texture(myTextureSampler, vsData.texCoord).xyz;

  // Occlusion
  float occlusion = texture(aoTextureSampler, vsData.texCoord).r;
  float ambiant = ka * occlusion;

  // Use everything in camera space
  color = vec4(phong(normalize(normal_tbn), 
                     normalize(vsData.light_tbn), 
                     -normalize(vsData.cam_tbn),
                    color_mat, vec3(1.0, 1.0, 1.0), 
                      ambiant, kd, ks, shininess), 1.0);
  
}