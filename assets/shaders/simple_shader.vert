#version 330 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
layout(location=0) in vec3 Vertex;

void main(void) {
    gl_Position = Projection * ((View * Model) * vec4(Vertex, 1.0));
}
