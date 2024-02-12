#version 330

in vec2 in_pos;
in vec2 in_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 transform;

out vec2 vert_tex_coord;

void main() {
    gl_Position = projection * view * model * transform * vec4(in_pos, 0.0, 1.0);
    vert_tex_coord = in_tex_coord;
}
