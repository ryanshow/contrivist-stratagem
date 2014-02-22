#version 330

layout(location=0) out vec4 FragColor;

layout (std140) uniform Window {
    mat4 projMatrix;
    mat4 viewMatrix;
} gWindow;

layout (std140) uniform Model {
    mat4 modelMatrix;
    uvec2 texSize;
} gModel;

uniform sampler2D gColorMap;

in VertexData {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 texture;
} gInData;

void main() {
    FragColor = gInData.color * texture(gColorMap, vec2(1.0-gInData.texture.s, 1.0-gInData.texture.t));

    // TODO: Implement proper alpha
    if (FragColor.a < 0.5) {
        discard;
    }
}
