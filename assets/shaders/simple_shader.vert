#version 330 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

layout(location=0) in vec3 Vertex;
layout(location=1) in vec3 Normal;
layout(location=2) in vec4 Color;
layout(location=3) in vec2 Texture;

out vec3 vPos;
out vec3 vNor;
out vec4 vCol;
out vec2 vTex;

void main(void) {
    gl_Position = Projection * ((View * Model) * vec4(Vertex, 1.0));
    vPos = Vertex;
    vNor = Normal;
    vCol = Color;
    vTex = Texture;
}
