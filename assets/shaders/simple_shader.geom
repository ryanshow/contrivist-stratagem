#version 330

layout (lines) in;
layout (line_strip,max_vertices=2) out;

layout (std140) uniform Window {
    mat4 projMatrix;
    mat4 viewMatrix;
} gWindow;

layout (std140) uniform Model {
    mat4 modelMatrix;
    uvec2 texSize;
} gModel;

in VertexData {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 texture;
} gInData[];

out VertexData {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 texture;
} gOutData;

void main() {
    for (int i=0; i<gl_in.length(); i++) {
        gl_Position = gl_in[i].gl_Position.xyzw;
        gOutData.position = gInData[i].position;
        gOutData.normal = gInData[i].normal;
        gOutData.color = gInData[i].color;
        gOutData.texture = gInData[i].texture;
        EmitVertex();
    }

    EndPrimitive();
}
