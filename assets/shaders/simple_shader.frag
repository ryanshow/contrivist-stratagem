#version 330 core

layout(location=0) out vec4 FragColor;

in VertexData {
    vec3 gPosition;
    vec3 gNormal;
    vec4 gColor;
    vec2 gTexture;
} inData;

void main(void) {
    FragColor = inData.gColor;
}
