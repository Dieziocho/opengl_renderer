#version 430 core
layout(location = 0) in vec3 position;
layout(std140, binding = 0) uniform Camera {
    mat4 projection_view;
    vec3 camera_position;
};

uniform mat4 model;

void main() {
    gl_Position = projection_view * model * vec4(position, 1.0);
}
