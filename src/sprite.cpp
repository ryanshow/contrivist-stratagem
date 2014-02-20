
// OpenGL related headers
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "format.h"

#include "lodepng.h"
#include "shader.h"
#include "sprite.h"
#include "vertex.h"

Sprite::Sprite() {
    // Set up the default shader for the object
    ShaderTypeNameMap shader_type_names;
    shader_type_names[GL_VERTEX_SHADER] = "simple_shader.vert";
    shader_type_names[GL_GEOMETRY_SHADER] = "billboard_shader.geom";
    shader_type_names[GL_FRAGMENT_SHADER] = "texture_shader.frag";
    this->shader = Shader::getShader(shader_type_names);

    // Set the default draw method
    this->draw_method = GL_POINTS;

    Vertex v[1] {};
    v[0].y = 0.0;
    v[0].r = 1.0f; v[0].g=1.0; v[0].b=1.0; v[0].a = 1.0f;

    int i[1] {0};

    this->setVertices(v, 1);
    this->setIndices(i, 1);
    this->bindBufferData();

    // Load up the image
    unsigned int error = lodepng::decode(this->image, this->textureSize[0], this->textureSize[1], "assets/images/black_mage.png");
    if (error != 0) {
        fmt::Print("Error: {0}\n") << lodepng_error_text(error);
    }

    glGenTextures(1, &(this->textureId));

    glBindVertexArray(this->vao);
        glBindTexture(GL_TEXTURE_2D, this->textureId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->textureSize.x, this->textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(this->image[0]));
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Sprite::render(Scene* scene) {

    // Make our vertex array active
    glBindVertexArray(this->vao);
        // Tell the renderer to use our shader program when rendering our object
        glUseProgram(this->shader->program_id);
            this->bindMatrixData(scene, M_PROJECTION|M_VIEW|M_MODEL);

            // Bind the texture data
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->textureId);
            glUniform1i(glGetUniformLocation(this->shader->program_id, "gColorMap"), 0);
            glUniform2uiv(glGetUniformLocation(this->shader->program_id, "gTexSize"), 1, glm::value_ptr(this->textureSize));

            // Render the vao on the screen using "GL_LINE_LOOP"
            glDrawElements(
                this->draw_method,
                this->index_list.size(),
                GL_UNSIGNED_SHORT,
                (void*)0);
        glUseProgram(0);
    glBindVertexArray(0);
}
