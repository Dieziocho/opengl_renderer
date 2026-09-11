#version 430 core
in vec2 texture_coordinates;
uniform sampler2D mesh_texture;

out vec4 frag_color;

void main() {
    frag_color = texture(mesh_texture, texture_coordinates);
}
