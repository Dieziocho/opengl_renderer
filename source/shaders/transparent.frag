#version 430 core
in vec2 texture_coordinates;
uniform sampler2D mesh_texture;

layout(location = 0) out vec4 accumulate;
layout(location = 1) out float reveal;

void main() {
    vec4 color = texture(mesh_texture, texture_coordinates);

    //Weight function
    float weight = clamp(pow(min(1.0, color.a * 10.0) + 0.01, 3.0) * 1e8 *
                pow(1.0 - gl_FragCoord.z * 0.9, 3.0), 1e-2, 3e3);

    //Store pixel color accumulation, blend function = GL_ONE GL_ONE GL_ZERO GL_ONE_MINUS_SRC_COLOR
    accumulate = vec4(color.rgb * color.a, color.a) * weight;

    //Store pixel revealage threshold, blend function = GL_ZERO GL_ONE_MINUS_SRC_COLOR
    reveal = color.a;
}
