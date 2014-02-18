#version 330

layout (points) in;
layout (triangle_strip,max_vertices=4) out;

uniform mat4 gProjection;
uniform mat4 gView;
uniform mat4 gModel;

uniform uvec2 gTexSize;

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
    vec3 camPos = -gView[3].xyz * mat3(gView);
    vec3 vertPos = inData[0].gPosition.xyz;
    float ratio = float(gTexSize.x)/float(gTexSize.y);

    outData.gPosition = inData[0].gPosition;
    outData.gNormal = inData[0].gNormal;
    outData.gColor = inData[0].gColor;

    vec3 toCamVec = normalize(camPos - vertPos);
    vec3 upVec = vec3(0.0, 1.0, 0.0);

    // TODO: Normalize this cross-product
    vec3 rightVec = normalize(cross(toCamVec, upVec));

    vertPos -= (rightVec * ratio / 2);
    gl_Position = gProjection * ((gView * gModel) * vec4(vertPos, 1.0));
    outData.gTexture = vec2(0.0, 0.0);
    EmitVertex();

    vertPos.y += 1.0;
    gl_Position = gProjection * ((gView * gModel) * vec4(vertPos, 1.0));
    outData.gTexture = vec2(0.0, 1.0);
    EmitVertex();

    vertPos.y -= 1.0;
    vertPos += (rightVec * ratio);
    gl_Position = gProjection * ((gView * gModel) * vec4(vertPos, 1.0));
    outData.gTexture = vec2(1.0, 0.0);
    EmitVertex();

    vertPos.y += 1.0;
    gl_Position = gProjection * ((gView * gModel) * vec4(vertPos, 1.0));
    outData.gTexture = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}

