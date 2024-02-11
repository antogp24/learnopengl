#version 330 core

in vec3 in_pos;
in vec2 in_tex_coord;

uniform mat4 transform;

out vec2 vert_tex_coord;

void main() {
   gl_Position = vec4(in_pos, 1.0) * transform;
   vert_tex_coord = in_tex_coord;
}
