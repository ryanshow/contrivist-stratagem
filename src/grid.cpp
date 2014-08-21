#include <iostream>

#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "format.h"

#include "grid.h"
#include "scene.h"
#include "window.h"

Grid::Grid() {
    this->setStep(20, 0.5f);
}

void Grid::setStep(int size, float step) {
    this->step = step;
    this->size = size;

    mVertexList.clear();

    Vertex v[4] {};
    v[0].col = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    v[1].col = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    v[1].pos.x = float(size);

    // Vertices used for the centerline
    v[2].col = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    v[3].col = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    v[3].pos.x = float(size);

    GLuint i[4] {0, 1, 2, 3};

    this->setVertices(v, 4);
    this->setIndices(i, 4);
    this->bindBufferData();
}

void Grid::render(const Window& window, const Scene& scene) {
    glBindBufferRange(GL_UNIFORM_BUFFER, BaseObject::msUBOBindingIndex, mpBufferObjects[UNIFORM], 0, sizeof(glm::mat4)+sizeof(glm::uvec2));
    // Make our vertex array active
    glBindVertexArray(mVAO);
        // Tell the renderer to use our shader program when rendering our object
        glUseProgram(mpShader->mProgramId);
            // Draw the gridlines. Starting at the center and going out in both directions.
            mModelMatrixStack.pushMatrix(); // Push 1
            mModelMatrixStack.setMatrix(glm::translate(mModelMatrixStack.getMatrix(), glm::vec3(-(this->size/2.0), 0.0f, 0.0f)));
            for (int i=-1; i<=1; i+=2) {
                mModelMatrixStack.pushMatrix(); // Push 2
                for (int j=0; j<int(this->size/this->step)/2; j++) {
                    mModelMatrixStack.setMatrix(glm::translate(mModelMatrixStack.getMatrix(), glm::vec3(0.0f, 0.0f, i*this->step)));
                    bindMatrixData();
                    // Render the vao on the screen
                    glDrawElements(
                        mDrawMethod,
                        2,
                        GL_UNSIGNED_INT,
                        (GLuint*)0);
                }
                mModelMatrixStack.popMatrix(); // Pop 2
            }
            mModelMatrixStack.popMatrix(); // Pop 1

            mModelMatrixStack.pushMatrix(); // Push 1
            mModelMatrixStack.setMatrix(glm::rotate(mModelMatrixStack.getMatrix(), float(M_PI/2), glm::vec3(0.0f, -1.0f, 0.0f)));
            mModelMatrixStack.setMatrix(glm::translate(mModelMatrixStack.getMatrix(), glm::vec3(-(this->size/2.0), 0.0f, 0.0f)));
            for (int i=-1; i<=1; i+=2) {
                mModelMatrixStack.pushMatrix(); // Push 2
                for (int j=0; j<int(this->size/this->step)/2; j++) {
                    mModelMatrixStack.setMatrix(glm::translate(mModelMatrixStack.getMatrix(), glm::vec3(0.0f, 0.0f, i*this->step)));
                    bindMatrixData();
                    // Render the vao on the screen
                    glDrawElements(
                        mDrawMethod,
                        2,
                        GL_UNSIGNED_INT,
                        (GLuint*)0);
                }
                mModelMatrixStack.popMatrix(); // Pop 2
            }
            mModelMatrixStack.popMatrix(); // Pop 1

            // Disable depth testing for the centerlines
            int old_depth_func;
            glGetIntegerv(GL_DEPTH_FUNC, &old_depth_func);

            glDepthFunc(GL_ALWAYS);

            // Draw the centerlines
            mModelMatrixStack.pushMatrix(); // Push 1
            mModelMatrixStack.setMatrix(glm::translate(mModelMatrixStack.getMatrix(), glm::vec3(-(this->size/2.0), 0.0f, 0.0f)));
            bindMatrixData();
            glDrawElements(
                mDrawMethod,
                2,
                GL_UNSIGNED_INT,
                (GLuint *)0 + 2);
            mModelMatrixStack.popMatrix(); // Pop 1

            mModelMatrixStack.pushMatrix(); // Push 1
            mModelMatrixStack.setMatrix(glm::rotate(mModelMatrixStack.getMatrix(), float(M_PI/2), glm::vec3(0.0f, -1.0f, 0.0f)));
            mModelMatrixStack.setMatrix(glm::translate(mModelMatrixStack.getMatrix(), glm::vec3(-(this->size/2.0), 0.0f, 0.0f)));
            bindMatrixData();
            glDrawElements(
                mDrawMethod,
                2,
                GL_UNSIGNED_INT,
                (GLuint *)0 + 2);
            mModelMatrixStack.popMatrix(); // Pop 1

            // Return depth testing to what it was before we disabled it.
            glDepthFunc(old_depth_func);
        glUseProgram(0);
    glBindVertexArray(0);
}
