
#version 330

in vec3 in_pos;
in vec2 in_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vert_tex_coord;

void main() {
    gl_Position = projection * view * model * vec4(in_pos, 1.0);
    vert_tex_coord = in_tex_coord;
}
