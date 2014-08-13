#version 330

layout (triangles) in;
layout (triangle_strip,max_vertices=3) out;

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

in vData {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 tex0;
} gVertices[];

out fData {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 tex0;
} gFrag;

void main() {
    for (int i=0; i<gl_in.length(); i++) {
        gl_Position = gWindow.projMatrix * gWindow.viewMatrix * gModel.modelMatrix * gl_in[i].gl_Position;
        gFrag.position = gVertices[i].position.xyz;
        gFrag.normal = gVertices[i].normal;
        gFrag.color = gVertices[i].color;
        gFrag.tex0 = gVertices[i].tex0;
        EmitVertex();
    }

    EndPrimitive();
}
