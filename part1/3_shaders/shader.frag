#version 330 core

out vec4 FragColor;
in vec3 customColor;
uniform float t;

void main() {
   vec4 color = vec4(customColor, 1.0);
   color.b += t;
   FragColor = color;
}
