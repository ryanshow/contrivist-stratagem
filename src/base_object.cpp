#include <GLFW/glfw3.h>

#include "base_object.h"
#include "shader.h"

BaseObject::BaseObject() {
    ShaderTypeNameMap shader_type_names;
    shader_type_names[GL_VERTEX_SHADER] = "simple_shader.vert";
    shader_type_names[GL_FRAGMENT_SHADER] = "simple_shader.frag";
    this->shader = Shader::getShader(shader_type_names);
}
