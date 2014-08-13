#version 330 core

layout(location=0) out vec4 FragColor;

layout (std140) uniform Window {
    mat4 projMatrix;
    mat4 viewMatrix;
    uvec2 winSize;
    uvec2 padding;
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

float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(void) {
    FragColor = gFrag.color * rand(gFrag.position.xy);
}
