#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 a_texture_coordinates;
layout(location = 2) in ivec4 bone_ids;
layout(location = 3) in vec4 weights;
layout(std140, binding = 0) uniform Camera {
    mat4 projection_view;
    vec3 camera_position;
};

layout(std430, binding = 0) readonly buffer BonesTransform {
    mat4 bones_transform[];
};
layout(std430, binding = 1) readonly buffer BonesOffset {
    mat4 bones_offset[];
};

uniform mat4 model;
out vec2 texture_coordinates;

void main() {
    vec4 total_position = vec4(0);
    for (int i = 0; i < 4; i++) {
        if (bone_ids[i] == -1) {
            if (i == 0)
                total_position = vec4(position, 1);
            break;
        }

        uint index = bone_ids[i];
        mat4 transform = bones_transform[index] * bones_offset[index];
        total_position += (transform * vec4(position, 1)) * weights[i];
    }

    gl_Position = projection_view * model * total_position;
    texture_coordinates = a_texture_coordinates;
}
