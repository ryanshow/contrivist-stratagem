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
    noperspective vec3 dist;
} gFrag;

void main(void) {
    float nearD = min(min(gFrag.dist.x, gFrag.dist.y), gFrag.dist.z);
    float edgeIntensity = exp2(-1.0*nearD*nearD);
    FragColor = vec4(vec3(1.0) - vec3(edgeIntensity), 1.0) * gFrag.color;
}
