#version 330

layout (lines) in;
layout (line_strip,max_vertices=2) out;

uniform mat4 gProjection;
uniform mat4 gView;
uniform mat4 gModel;

in VertexData {
    vec3 gPosition;
    vec3 gNormal;
    vec4 gColor;
    vec2 gTexture;
} inData[];

out VertexData {
    vec3 gPosition;
    vec3 gNormal;
    vec4 gColor;
    vec2 gTexture;
} outData;

void main() {
    for (int i=0; i<gl_in.length(); i++) {
        gl_Position = gl_in[i].gl_Position.xyzw;
        outData.gPosition = inData[i].gPosition;
        outData.gNormal = inData[i].gNormal;
        outData.gColor = inData[i].gColor;
        outData.gTexture = inData[i].gTexture;
        EmitVertex();
    }

    EndPrimitive();
}
