#version 330 core

in vec3 vert_color;
in vec2 vert_tex_coord;

uniform float mix_t;
uniform sampler2D tex0;
uniform sampler2D tex1;

out vec4 frag_color;

void main() {
   vec4 t0 = texture(tex0, vert_tex_coord);
   vec4 t1 = texture(tex1, vec2(-vert_tex_coord.x, vert_tex_coord.y));
   frag_color = mix(t0, t1, mix_t) * vec4(vert_color, 1.0);
}
