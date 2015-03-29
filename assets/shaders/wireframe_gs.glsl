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
    noperspective vec3 dist;
} gFrag;

float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    vec4 positions[3];
    vec2 p[3];
    vec2 v[3];
    for (int i=0; i<gl_in.length(); i++) {
        positions[i] = gWindow.projMatrix * gWindow.viewMatrix * gModel.modelMatrix * gl_in[i].gl_Position;
        p[i] = gWindow.winSize * positions[i].xy/positions[i].w;
    }

    v[0] = p[2] - p[1];
    v[1] = p[2] - p[0];
    v[2] = p[1] - p[0];
    float area = abs(v[1].x*v[2].y - v[1].y*v[2].x);

    for (int i=0; i<3; i++) {
        gFrag.position = gVertices[i].position.xyz;
        gFrag.normal = gVertices[i].normal;
        gFrag.color = gVertices[i].color * vec4(rand(gFrag.position.xy), rand(gFrag.position.xz), rand(gFrag.position.yx), 1.0);
        gFrag.tex0 = gVertices[i].tex0;
        gFrag.dist = vec3(0.0);
        gFrag.dist[i] = area/length(v[i]);
        gl_Position = positions[i];
        EmitVertex();
    }

    EndPrimitive();
}
