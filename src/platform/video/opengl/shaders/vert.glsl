#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;

out vec2 vTexCoord;
out vec4 vColor;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vTexCoord = aTexCoord;
    vColor = aColor;
}
