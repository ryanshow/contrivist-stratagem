#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "format.h"

#include "base_object.h"
#include "scene.h"
#include "window.h"

BaseObject::BaseObject() {

    // Set up the default shader for the object
    ShaderTypeNameMap shader_type_names;
    shader_type_names[GL_VERTEX_SHADER] = "simple_shader.vert";
    shader_type_names[GL_FRAGMENT_SHADER] = "simple_shader.frag";
    //shader_type_names[GL_GEOMETRY_SHADER] = "simple_shader.geom";
    mpShader = Shader::getShader(shader_type_names);

    // Initialize the model transformation matrix
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::rotate(matrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, 0.0f));
    matrix = glm::scale(matrix, glm::vec3(1.0f, 1.0f, 1.0f));
    mModelMatrixStack.setMatrix(matrix);

    // Set the default draw method
    mDrawMethod = GL_LINES;

    // Create the Vertex Array Object
    glGenVertexArrays(1, &mVAO);

    // Create our buffer objects
    mpBufferObjects = new GLuint[3];
    glGenBuffers(3, mpBufferObjects);

    // VAOs allow us to declare all of these attrib pointers while the VAO is
    // bound. This reduces clutter in the render member function.
    glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mpBufferObjects[VERTEX]);
            // Position (XYZ)
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                (void*)offsetof(Vertex, pos));

            // Normal (XYZ)
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                (void*)offsetof(Vertex, nor));

            // Color (RGBA)
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(
                2,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                (void*)offsetof(Vertex, col));

            // Texture (ST)
            // TODO: I should bind the last 3 textures here and access it as an array in the shader
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(
                3,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                (void*)offsetof(Vertex, tx0));
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Bind the index buffer to the VAO, since that's what we'll use to render
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mpBufferObjects[INDEX]);
    glBindVertexArray(0);

    glBindBuffer(GL_UNIFORM_BUFFER, mpBufferObjects[UNIFORM]);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + sizeof(glm::uvec2), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mModelMatrixStack.getMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    mUBOBindingIndex = Window::createUniformBindingIndex();
    glUniformBlockBinding(
        mpShader->mProgramId,
        glGetUniformBlockIndex(mpShader->mProgramId, "Model"),
        this->mUBOBindingIndex);

}

void BaseObject::bindBufferData() {
    // Bind the vertex buffer data
    glBindBuffer(GL_ARRAY_BUFFER, mpBufferObjects[VERTEX]);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(Vertex)*mVertexList.size(),
            &(mVertexList)[0],
            GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind the Index buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mpBufferObjects[INDEX]);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(GLuint)*mIndexList.size(),
            &(mIndexList)[0],
            GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BaseObject::bindMatrixData(Window* window, Scene *scene, const unsigned char bind_mask) {
    if (bind_mask & M_PROJECTION || bind_mask & M_VIEW) {
        glUniformBlockBinding(
            mpShader->mProgramId,
            glGetUniformBlockIndex(mpShader->mProgramId, "Window"),
            window->mUBOBindingIndex);
        glBindBufferRange(GL_UNIFORM_BUFFER, window->mUBOBindingIndex, window->mUBO, 0, sizeof(glm::mat4) * 2);
    }
    if (bind_mask & M_MODEL) {
        // FIXME: This should only occur when the model is transformed
        glBindBuffer(GL_UNIFORM_BUFFER, mpBufferObjects[UNIFORM]);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mModelMatrixStack.getMatrix()));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // FIXME: It should not be necessary to bind this every frame.. find out why it's needed here.
        glUniformBlockBinding(
            mpShader->mProgramId,
            glGetUniformBlockIndex(mpShader->mProgramId, "Model"),
            mUBOBindingIndex);

        glBindBufferRange(GL_UNIFORM_BUFFER, mUBOBindingIndex, mpBufferObjects[UNIFORM], 0, sizeof(glm::mat4));
    }
}

void BaseObject::render(Window* window, Scene* scene) {

    // Make our vertex array active
    glBindVertexArray(mVAO);
        // Tell the renderer to use our shader program when rendering our object
        glUseProgram(mpShader->mProgramId);
            this->bindMatrixData(window, scene, M_PROJECTION|M_VIEW|M_MODEL);

            // Render the vao on the screen using "GL_LINE_LOOP"
            glDrawElements(
                mDrawMethod,
                mIndexList.size(),
                GL_UNSIGNED_SHORT,
                (void*)0);
        glUseProgram(0);
    glBindVertexArray(0);
}

void BaseObject::setVertices(Vertex* vert_list, int vert_count) {
    mVertexList.clear();
    for (int i=0; i<vert_count; i++) {
        mVertexList.push_back(vert_list[i]);
    }
}

void BaseObject::setIndices(int* pIndexList, int indexCount) {
    mIndexList.clear();
    for (int i=0; i<indexCount; i++) {
        mIndexList.push_back(pIndexList[i]);
    }
}