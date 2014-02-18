#version 330 core

uniform mat4 gModel;
uniform mat4 gView;
uniform mat4 gProjection;

layout(location=0) in vec3 gPosition;
layout(location=1) in vec3 gNormal;
layout(location=2) in vec4 gColor;
layout(location=3) in vec2 gTexture;

out VertexData {
    vec3 gPosition;
    vec3 gNormal;
    vec4 gColor;
    vec2 gTexture;
} outData;

void main(void) {
    gl_Position = gProjection * ((gView * gModel) * vec4(gPosition, 1.0));
    outData.gPosition = gPosition;
    outData.gNormal = gNormal;
    outData.gColor = gColor;
    outData.gTexture = gTexture;
}
