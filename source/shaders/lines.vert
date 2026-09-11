#version 430 core
layout(location = 0) in vec3 position;
layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
}
