#version 330

layout (points) in;
layout (triangle_strip,max_vertices=4) out;

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
    vec3 camPos = -gWindow.viewMatrix[3].xyz * mat3(gWindow.viewMatrix);
    vec3 vertPos = gInData[0].position.xyz;
    float ratio = float(gModel.texSize.x)/float(gModel.texSize.y);

    gOutData.position = gInData[0].position;
    gOutData.normal = gInData[0].normal;
    gOutData.color = gInData[0].color;

    vec3 toCamVec = normalize(camPos - vertPos);
    vec3 upVec = vec3(0.0, 1.0, 0.0);

    // TODO: Normalize this cross-product
    vec3 rightVec = normalize(cross(toCamVec, upVec));

    vertPos -= (rightVec * ratio / 2);
    gl_Position = gWindow.projMatrix * ((gWindow.viewMatrix * gModel.modelMatrix) * vec4(vertPos, 1.0));
    gOutData.texture = vec2(0.0, 0.0);
    EmitVertex();

    vertPos.y += 1.0;
    gl_Position = gWindow.projMatrix * ((gWindow.viewMatrix * gModel.modelMatrix) * vec4(vertPos, 1.0));
    gOutData.texture = vec2(0.0, 1.0);
    EmitVertex();

    vertPos.y -= 1.0;
    vertPos += (rightVec * ratio);
    gl_Position = gWindow.projMatrix * ((gWindow.viewMatrix * gModel.modelMatrix) * vec4(vertPos, 1.0));
    gOutData.texture = vec2(1.0, 0.0);
    EmitVertex();

    vertPos.y += 1.0;
    gl_Position = gWindow.projMatrix * ((gWindow.viewMatrix * gModel.modelMatrix) * vec4(vertPos, 1.0));
    gOutData.texture = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}

