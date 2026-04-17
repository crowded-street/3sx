#version 330 core

in vec4 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler1D uPalette;
uniform usampler2D uIndexTex;
uniform ivec2 uTextureSize;

uint fetch4bppIndex(vec2 uv) {
    ivec2 pixelCoord = ivec2(
        int(uv.x * float(uTextureSize.x)),
        int(uv.y * float(uTextureSize.y))
    );

    int packedX = pixelCoord.x / 2;
    uint packed = texelFetch(uIndexTex, ivec2(packedX, pixelCoord.y), 0).r;

    return ((pixelCoord.x & 1) == 0) ? (packed & 0xFu) : ((packed >> 4) & 0xFu);
}

void main() {
    uint index = fetch4bppIndex(TexCoord);
    FragColor = texelFetch(uPalette, int(index), 0);
}
