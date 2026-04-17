#version 330 core

in vec4 vColor;
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler1D uPalette;
uniform usampler2D uIndexTex;

void main() {
    uint index = texture(uIndexTex, vTexCoord).r;
    vec4 color = texelFetch(uPalette, int(index), 0);
    FragColor = color * vColor;

	if (FragColor.a == 0) {
		discard;
	}
}
