#version 330

in vec2 vert_tex_coord;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform int sign;

out vec4 frag_color;

void main() {
    vec4 tex0 = texture(texture0, vert_tex_coord);
    vec4 tex1 = texture(texture1, vert_tex_coord);

    frag_color = mix(tex0, tex1, 0.2);
}
