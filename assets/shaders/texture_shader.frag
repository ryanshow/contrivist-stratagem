#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D gColorMap;

in VertexData {
    vec3 gPosition;
    vec3 gNormal;
    vec4 gColor;
    vec2 gTexture;
} inData;

void main() {
    FragColor = inData.gColor * texture(gColorMap, vec2(1.0-inData.gTexture.s, 1.0-inData.gTexture.t));

    if (FragColor.a < 0.5) {
        discard;
    }
}
