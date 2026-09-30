#version 430 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 a_texture_coordinates;
layout(std140, binding = 0) uniform Camera {
    mat4 projection_view;
    vec3 camera_position;
};

uniform mat4 model;

out vec2 texture_coordinates;

void main() {
    gl_Position = projection_view * model * vec4(position, 0.0, 1.0);
    texture_coordinates = a_texture_coordinates;
}
