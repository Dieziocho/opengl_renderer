#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 a_texture_coordinates;
layout(location = 2) in ivec4 bone_ids;
layout(location = 3) in vec4 weights;
layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

struct Bone {
    mat4 transform;
};

layout(std430, binding = 0) readonly buffer Bones {
    Bone bones[];
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

        mat4 transform = bones[bone_ids[i]].transform;
        total_position += (transform * vec4(position, 1)) * weights[i];
    }

    gl_Position = projection * view * model * total_position;
    texture_coordinates = a_texture_coordinates;
}
