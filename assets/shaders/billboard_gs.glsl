#version 330

layout (points) in;
layout (triangle_strip,max_vertices=4) out;

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
} gOutFrag;

void main() {
    float scale = 0.3;

    vec3 camPos = -gWindow.viewMatrix[3].xyz * mat3(gWindow.viewMatrix);
    vec3 vertPos = gl_in[0].gl_Position.xyz;
    float ratio = float(gModel.texSize.x)/float(gModel.texSize.y);

    gOutFrag.position = gVertices[0].position;
    gOutFrag.normal = gVertices[0].normal;
    gOutFrag.color = gVertices[0].color;

    vec3 toCamVec = normalize(camPos - vertPos);
    vec3 upVec = vec3(0.0, 1.0, 0.0);
    vec3 rightVec = normalize(cross(toCamVec, upVec));

    vertPos -= ((rightVec * ratio / 2) * scale);
    gl_Position = gWindow.projMatrix * gWindow.viewMatrix * gModel.modelMatrix * vec4(vertPos, 1.0);
    gOutFrag.tex0 = gVertices[0].tex0 + vec2(1.0, 1.0);
    EmitVertex();

    vertPos.y += (1.0 * scale);
    gl_Position = gWindow.projMatrix * gWindow.viewMatrix * gModel.modelMatrix * vec4(vertPos, 1.0);
    gOutFrag.tex0 = gVertices[0].tex0 + vec2(1.0, 0.0);
    EmitVertex();

    vertPos.y -= (1.0 * scale);
    vertPos += ((rightVec * ratio) * scale);
    gl_Position = gWindow.projMatrix * gWindow.viewMatrix * gModel.modelMatrix * vec4(vertPos, 1.0);
    gOutFrag.tex0 = gVertices[0].tex0 + vec2(0.0, 1.0);
    EmitVertex();

    vertPos.y += (1.0 * scale);
    gl_Position = gWindow.projMatrix * gWindow.viewMatrix * gModel.modelMatrix * vec4(vertPos, 1.0);
    gOutFrag.tex0 = gVertices[0].tex0;
    EmitVertex();

    EndPrimitive();
}

