#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec2 texcoord;

const int MAX_LIGHTS = 10;

out VertexData
{
    vec3 position_view; //position in camera space
    vec3 normal_view; //normal in camera space
    vec3 tangent_view; //tangent in camera space
    vec3 bitangent_view; //bitangent in camera space
    vec2 texCoord; //texture coordinate
    vec3 light_view[MAX_LIGHTS]; // light direction in camera space
    vec3 light_tbn[MAX_LIGHTS]; // light direction in camera space in tangent space
    vec3 cam_tbn; // camera direction in camera space
} vsData;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Perspective;

uniform vec3 Light_list[MAX_LIGHTS];

uniform sampler2D sampler_displacement;

void main()
{
    vec3 position_displacement = position + texture(sampler_displacement, texcoord).x * 0.2 * normal;

    gl_Position = Perspective * View * Model * vec4(position_displacement, 1.0);

    // Send all information in camera space
    vsData.position_view = (View * Model * vec4(position_displacement, 1.0)).xyz;
    // For explanation https://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
    vsData.normal_view = (transpose(inverse(View * Model)) * vec4(normal, 0.0)).xyz;
    vsData.tangent_view = normalize((View * Model * vec4(tangent.xyz, 0.0)).xyz);
    vsData.bitangent_view = normalize(cross(vsData.tangent_view, vsData.normal_view)*tangent.w);

    vsData.texCoord = texcoord;

    // Initialiser la lumière vue à zéro
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        vsData.light_view[i] = vec3(0.0);
    }

    // Boucle pour gérer chaque lumière dans Light_list
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        vec3 light_pos_view = (View * vec4(Light_list[i], 1.0)).xyz;
        vsData.light_view[i] = normalize(light_pos_view - vsData.position_view);

        // Matrice de passage du repère view au repère tangent
        mat3 TBN = transpose(mat3(vsData.tangent_view, vsData.bitangent_view, vsData.normal_view));
        vsData.light_tbn[i] = TBN * vsData.light_view[i];
    }

    vsData.cam_tbn = transpose(mat3(vsData.tangent_view, vsData.bitangent_view, vsData.normal_view)) * vsData.position_view;

    // vec3 light_pos_view = (View * vec4(light_pos, 1.0)).xyz;
    // vsData.light_view = normalize(light_pos_view - vsData.position_view);


    // vec3 b = normalize(cross(vsData.tangent_view, vsData.normal_view)*tangent.w);
    // // Matrice de passage du repere view au repere tangant
    // mat3 TBN = transpose(mat3(vsData.tangent_view, b, vsData.normal_view ));
    // vsData.light_tbn = TBN * vsData.light_view;
    // vsData.cam_tbn = TBN * vsData.position_view;
}