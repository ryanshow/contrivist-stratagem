#version 330 core

layout(location=0) out vec4 FragColor;

in vec3 vPos;
in vec3 vNor;
in vec4 vCol;
in vec2 vTex;

void main(void) {
    FragColor = vCol;
}
