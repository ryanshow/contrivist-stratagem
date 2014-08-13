#include <iostream>

// OpenGL related headers
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "format.h"

#include "lodepng.h"
#include "shader.h"
#include "sprite.h"
#include "window.h"

Sprite::Sprite(const glm::vec3 pos) {
    // Set up the default shader for the object
    ShaderTypeNameMap shader_type_names;
    shader_type_names[GL_VERTEX_SHADER] = "simple_shader.vert";
    shader_type_names[GL_GEOMETRY_SHADER] = "billboard_shader.geom";
    shader_type_names[GL_FRAGMENT_SHADER] = "texture_shader.frag";
    mpShader = Shader::getShader(shader_type_names);

    // Set the default draw method
    mDrawMethod = GL_POINTS;

    Vertex v[1] {};
    v[0].pos = pos;
    v[0].col = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    v[0].tx0 = glm::vec2(0.0, 0.0);

    int i[1] {0};

    this->setVertices(v, 1);
    this->setIndices(i, 1);
    this->bindBufferData();

    // Load up the image
    unsigned int error = lodepng::decode(mTextureData, mTextureSize[0], mTextureSize[1], "assets/images/charmander.png");
    if (error != 0) {
        fmt::Print("Error: {0}\n") << lodepng_error_text(error);
    }

    glGenTextures(1, &(mTextureId));

    glBindVertexArray(mVAO);
        glBindTexture(GL_TEXTURE_2D, mTextureId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureSize.x, mTextureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(mTextureData[0]));
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_UNIFORM_BUFFER, mpBufferObjects[UNIFORM]);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::uvec2), glm::value_ptr(mTextureSize));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Sprite::render(const Window & window, const Scene & scene) {
    glBindBufferRange(GL_UNIFORM_BUFFER, BaseObject::msUBOBindingIndex, mpBufferObjects[UNIFORM], 0, sizeof(glm::mat4)+sizeof(glm::uvec2));

    // Make our vertex array active
    glBindVertexArray(mVAO);
        // Tell the renderer to use our shader program when rendering our object
        glUseProgram(mpShader->mProgramId);
            // Bind the texture data
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mTextureId);
            glUniform1i(glGetUniformLocation(mpShader->mProgramId, "gColorMap"), 0);

            // Render the vao on the screen using "GL_POINTS"
            glDrawElements(
                mDrawMethod,
                mIndexList.size(),
                GL_UNSIGNED_SHORT,
                (void*)0);

        glUseProgram(0);
    glBindVertexArray(0);
}
