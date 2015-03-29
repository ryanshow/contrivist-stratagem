#include <GLFW/glfw3.h>

#include "format.h"

#include "base_object.h"
#include "scene.h"
#include "window.h"

GLuint BaseObject::msUBOBindingIndex = 1;

BaseObject::BaseObject() {

    // Set up the default shader for the object
    ShaderTypeNameMap shader_type_names;
    shader_type_names[GL_VERTEX_SHADER] = "simple_vs.glsl";
    shader_type_names[GL_FRAGMENT_SHADER] = "simple_fs.glsl";
    shader_type_names[GL_GEOMETRY_SHADER] = "lines_gs.glsl";
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
                (GLvoid*)offsetof(Vertex, pos));

            // Normal (XYZ)
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                (GLvoid*)offsetof(Vertex, nor));

            // Color (RGBA)
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(
                2,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                (GLvoid*)offsetof(Vertex, col));

            // Texture0 (ST)
            // TODO: I should bind the last 3 textures here and access it as an array in the shader
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(
                3,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                (GLvoid*)offsetof(Vertex, tx0));

            // Texture1 (ST)
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(
                4,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                (GLvoid*)offsetof(Vertex, tx1));

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Bind the index buffer to the VAO, since that's what we'll use to render
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mpBufferObjects[INDEX]);
    glBindVertexArray(0);

    glBindBuffer(GL_UNIFORM_BUFFER, mpBufferObjects[UNIFORM]);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + sizeof(glm::uvec2) + sizeof(glm::uvec2), NULL, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mModelMatrixStack.getMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void BaseObject::bindBufferData() const {
    glPrimitiveRestartIndex(RESTART_INDEX);
    // Bind the vertex buffer data
    glBindBuffer(GL_ARRAY_BUFFER, mpBufferObjects[VERTEX]);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(Vertex)*mVertexList.size(),
            mVertexList.data(),
            GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind the Index buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mpBufferObjects[INDEX]);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(GLuint)*mIndexList.size(),
            mIndexList.data(),
            GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BaseObject::bindMatrixData() const {
    glBindBuffer(GL_UNIFORM_BUFFER, mpBufferObjects[UNIFORM]);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mModelMatrixStack.getMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void BaseObject::render(const Window & window, const Scene & scene) {
    // This binds the mpBufferObjects[UNIFORM] UBO to the binding index at mUBOBindingIndex
    glBindBufferRange(GL_UNIFORM_BUFFER, BaseObject::msUBOBindingIndex, mpBufferObjects[UNIFORM], 0, sizeof(glm::mat4)+sizeof(glm::uvec2));

    // Make our vertex array active
    glBindVertexArray(mVAO);
        // Tell the renderer to use our shader program when rendering our object
        glUseProgram(mpShader->mProgramId);
            // Render the vao on the screen using "GL_LINE_LOOP"
            glDrawElements(
                mDrawMethod,
                mIndexList.size(),
                GL_UNSIGNED_INT,
                (GLuint*)0);
        glUseProgram(0);
    glBindVertexArray(0);
}

void BaseObject::setVertices(Vertex* vert_list, int vert_count) {
    mVertexList.clear();
    for (int i=0; i<vert_count; i++) {
        mVertexList.push_back(vert_list[i]);
    }
}

void BaseObject::setIndices(GLuint* pIndexList, int indexCount) {
    mIndexList.clear();
    for (int i=0; i<indexCount; i++) {
        mIndexList.push_back(pIndexList[i]);
    }
}
