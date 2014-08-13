#version 330 core

layout (std140) uniform Window {
    mat4 projMatrix;
    mat4 viewMatrix;
} gWindow;

layout (std140) uniform Model {
    mat4 modelMatrix;
    uvec2 texSize;
    uvec2 padding;
} gModel;

layout(location=0) in vec3 gPosition;
layout(location=1) in vec3 gNormal;
layout(location=2) in vec4 gColor;
layout(location=3) in vec2 gTexture0;
layout(location=4) in vec2 gTexture1;

out vData {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 tex0;
} gVertex;

void main(void) {
    gl_Position = vec4(gPosition, 1.0);
    gVertex.position = gPosition;
    gVertex.normal = gNormal.xyz;
    gVertex.color = gColor;
    gVertex.tex0 = gTexture0;
}
