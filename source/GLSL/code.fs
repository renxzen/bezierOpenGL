#version 330 core
out vec4 FragColor;

uniform vec3 colorize;

void main() {
   FragColor = vec4(colorize, 1.0);
}