#version 330

layout(location=0) out vec4 FragColor;

layout (std140) uniform Window {
    mat4 projMatrix;
    mat4 viewMatrix;
} gWindow;

layout (std140) uniform Model {
    mat4 modelMatrix;
    uvec2 texSize;
    uvec2 padding;
} gModel;

uniform sampler2D gColorMap;

in fData {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 tex0;
} gFrag;

void main() {
    FragColor = gFrag.color * texture(gColorMap, vec2(1.0-gFrag.tex0.s, 1.0-gFrag.tex0.t));

    // TODO: Implement proper alpha
    if (FragColor.a < 0.5) {
        discard;
    }
}
