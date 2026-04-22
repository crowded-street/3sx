#version 330 core

in vec4 vColor;
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, vTexCoord) * vColor;

	if (FragColor.a == 0) {
		discard;
	}
}
