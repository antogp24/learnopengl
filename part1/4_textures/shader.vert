#version 330 core

in vec3 in_pos;
in vec3 in_color;
in vec2 in_tex_coord;

out vec3 vert_color;
out vec2 vert_tex_coord;

void main() {
   gl_Position = vec4(in_pos, 1.0);
   vert_color = in_color;
   vert_tex_coord = in_tex_coord;
}
