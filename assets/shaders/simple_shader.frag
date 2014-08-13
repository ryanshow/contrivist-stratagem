#version 330 core

layout(location=0) out vec4 FragColor;

layout (std140) uniform Window {
    mat4 projMatrix;
    mat4 viewMatrix;
} gWindow;

layout (std140) uniform Model {
    mat4 modelMatrix;
    uvec2 texSize;
    uvec2 padding;
} gModel;

in fData {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 tex0;
} gFrag;

void main(void) {
    FragColor = gFrag.color;
}
