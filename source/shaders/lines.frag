#version 430 core
uniform vec3 line_color = vec3(1, 0, 0);

out vec4 frag_color;

void main() {
    frag_color = vec4(line_color, 1);
}
