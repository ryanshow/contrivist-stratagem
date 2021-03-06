#include <cstring>

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include <format.h>

#include <GLFW/glfw3.h>

#include "shader.h"

//Initialize the static class variables
std::map<ShaderTypeName,GLuint> Shader::msShaderIdMap = std::map<ShaderTypeName,GLuint>{};
std::map<ShaderTypeNameMap,Shader> Shader::msShaderProgramMap = std::map<ShaderTypeNameMap,Shader>{};


Shader::Shader(const ShaderTypeNameMap & shader_names) {

    // Create the shader program and grab it's ID
    mProgramId = glCreateProgram();

    GLuint shader_id;
    for (auto &shader_type_name : shader_names) {
        ShaderTypeName shader_pair(shader_type_name.first, shader_type_name.second);
        shader_id = addShader(shader_pair);
        glAttachShader(mProgramId, shader_id);
    }

    glLinkProgram(mProgramId);

    int link_success;
    glGetProgramiv(mProgramId, GL_LINK_STATUS, &link_success);
    if (link_success) {
        glUniformBlockBinding(
            mProgramId,
            glGetUniformBlockIndex(mProgramId, "Model"),
            1);
        glUniformBlockBinding(
            mProgramId,
            glGetUniformBlockIndex(mProgramId, "Window"),
            0);
    } else {
        // Get the length of the error message
        GLint length;
        glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &length);

        // Get the error message text
        GLchar* log = new GLchar[length + 1];
        glGetProgramInfoLog(mProgramId, length, NULL, log);

        // Print the message out
        fmt::Print("Error linking shader program: {0}") << log;

        // TODO: Do we raise an exception or something at this point?
    }
}


GLuint Shader::addShader(const ShaderTypeName & shader_type_name) {

    // Only bother creating this shader if we haven't compiled it before
    //if (Shader::msShaderIdMap.count(shader_type_name) == 0) {

        // Create the new shader id
        GLuint shader_id = glCreateShader(shader_type_name.first);

        // Load the shader source
        const char* shader_source = this->loadSource(fmt::c_str(fmt::Format("assets/shaders/{0}") << shader_type_name.second));

        // Assign the source to the shader we created earlier
        glShaderSource(shader_id, 1, &shader_source, nullptr);

        // Compile the shader
        glCompileShader(shader_id);

        // Check to see if there were any errors in compiling the shader
        int compile_success;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_success);
        if (!compile_success) {
            // Get the length of the error message
            GLint length;
            glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

            // Get the error message text
            GLchar* log = new GLchar[length + 1];
            glGetShaderInfoLog(shader_id, length, NULL, log);

            // Print the message out
            fmt::Print("Error compiling {0}: {1}") << shader_type_name.second << log;

            // TODO: Do we raise an exception or something at this point?
        }
        // Delete the source so we don't leak memory
        delete [] shader_source;

        // Add the shader id to the shader cache map
        Shader::msShaderIdMap[shader_type_name] = shader_id;

        return shader_id;
    //}

    //return Shader::msShaderIdMap[shader_type_name];
}


Shader* Shader::getShader(const ShaderTypeNameMap &shader_type_names) {
    if (Shader::msShaderProgramMap.count(shader_type_names) == 0) {
        Shader::msShaderProgramMap[shader_type_names] = Shader(shader_type_names);
    }
    return &Shader::msShaderProgramMap[shader_type_names];
}


const char* Shader::loadSource(const char* file_name) {
    std::ifstream file;
    std::ostringstream file_content_stream;
    std::string file_content;

    file.open(file_name);

    if (file.is_open()) {
        file_content_stream << file.rdbuf();
        file_content = file_content_stream.str();
        file.close();
    }

    char* return_data = new char[file_content.length()+1];
    strcpy(return_data, file_content.c_str());

    return return_data;
}
