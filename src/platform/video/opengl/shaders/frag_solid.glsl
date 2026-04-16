#version 330 core

in vec4 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

void main() {
    FragColor = ourColor;
}
