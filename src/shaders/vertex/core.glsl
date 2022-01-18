#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coord;
layout (location = 2) in vec3 normal;

out vec3 vs_pos;
out vec2 vs_texture_coord;
out vec3 vs_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    vs_pos = vec4(model_matrix * vec4(position, 1.f)).xyz;
    vs_texture_coord = vec2(texture_coord.x, texture_coord.y * -1.f);
    vs_normal = mat3(model_matrix) * normal;
    
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.f);
}