#version 330 core

in vec2 aPos;
in vec3 aColor;
out vec3 customColor;

void main() {
   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
   customColor = aColor;
}
