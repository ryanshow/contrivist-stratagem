#version 330 core

layout (std140) uniform Window {
    mat4 projMatrix;
    mat4 viewMatrix;
} gWindow;

layout (std140) uniform Model {
    mat4 modelMatrix;
    uvec2 texSize;
} gModel;

layout(location=0) in vec3 gPosition;
layout(location=1) in vec3 gNormal;
layout(location=2) in vec4 gColor;
layout(location=3) in vec2 gTexture;

out VertexData {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 texture;
} gOutData;

void main(void) {
    gl_Position = gWindow.projMatrix * ((gWindow.viewMatrix * gModel.modelMatrix) * vec4(gPosition, 1.0));
    gOutData.position = gPosition;
    gOutData.normal = gNormal;
    gOutData.color = gColor;
    gOutData.texture = gTexture;
}
