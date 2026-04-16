#version 330 core

in vec4 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler1D uPalette;
uniform usampler2D uIndexTex;

void main() {
    uint index = texture(uIndexTex, TexCoord).r;
    vec4 color = texelFetch(uPalette, int(index), 0);
    FragColor = color * ourColor;
}
