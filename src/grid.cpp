#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
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

    this->vertex_list.clear();

    Vertex v[4] {};
    v[0].r = 1.0f; v[0].a = 1.0f;
    v[1].x = float(size); v[1].r = 1.0f; v[1].g = 1.0f; v[1].a = 1.0f;

    // Vertices used for the centerline
    v[2].b=1.0f; v[2].a = 1.0f;
    v[3].x = float(size); v[3].a = 1.0f;

    int i[4] {0, 1, 2, 3};

    this->setVertices(v, 4);
    this->setIndices(i, 4);
    this->bindBufferData();
}

void Grid::render(Window* window, Scene* scene) {
    // Make our vertex array active
    glBindVertexArray(this->vao);
        // Tell the renderer to use our shader program when rendering our object
        glUseProgram(this->shader->program_id);
            // Bind the Projection/View matricies to the shader
            this->bindMatrixData(window, scene, M_PROJECTION|M_VIEW);

            // Draw the gridlines. Starting at the center and going out in both directions.
            this->pushModelMatrix(); // Push 1
            this->setModelMatrix(glm::translate(this->getModelMatrix(), glm::vec3(-(this->size/2.0), 0.0f, 0.0f)));
            for (int i=-1; i<=1; i+=2) {
                this->pushModelMatrix(); // Push 2
                for (int j=0; j<int(this->size/this->step)/2; j++) {
                    this->setModelMatrix(glm::translate(this->getModelMatrix(), glm::vec3(0.0f, 0.0f, i*this->step)));
                    this->bindMatrixData(window, scene, M_MODEL);
                    // Render the vao on the screen
                    glDrawElements(
                        this->draw_method,
                        2,
                        GL_UNSIGNED_SHORT,
                        (void*)0);
                }
                this->popModelMatrix(); // Pop 2
            }
            this->popModelMatrix(); // Pop 1

            this->pushModelMatrix(); // Push 1
            this->setModelMatrix(glm::rotate(this->getModelMatrix(), float(M_PI/2), glm::vec3(0.0f, -1.0f, 0.0f)));
            this->setModelMatrix(glm::translate(this->getModelMatrix(), glm::vec3(-(this->size/2.0), 0.0f, 0.0f)));
            for (int i=-1; i<=1; i+=2) {
                this->pushModelMatrix(); // Push 2
                for (int j=0; j<int(this->size/this->step)/2; j++) {
                    this->setModelMatrix(glm::translate(this->getModelMatrix(), glm::vec3(0.0f, 0.0f, i*this->step)));
                    this->bindMatrixData(window, scene, M_MODEL);
                    // Render the vao on the screen
                    glDrawElements(
                        this->draw_method,
                        2,
                        GL_UNSIGNED_SHORT,
                        (void*)0);
                }
                this->popModelMatrix(); // Pop 2
            }
            this->popModelMatrix(); // Pop 1

            // Disable depth testing for the centerlines
            int old_depth_func;
            glGetIntegerv(GL_DEPTH_FUNC, &old_depth_func);

            glDepthFunc(GL_ALWAYS);

            // Draw the centerlines
            this->pushModelMatrix(); // Push 1
            this->setModelMatrix(glm::translate(this->getModelMatrix(), glm::vec3(-(this->size/2.0)+0.1, 0.0f, 0.0f)));
            this->bindMatrixData(window, scene, M_MODEL);
            glDrawElements(
                this->draw_method,
                2,
                GL_UNSIGNED_SHORT,
                (char *)NULL + (4));
            this->popModelMatrix(); // Pop 1

            this->pushModelMatrix(); // Push 1
            this->setModelMatrix(glm::rotate(this->getModelMatrix(), float(M_PI/2), glm::vec3(0.0f, -1.0f, 0.0f)));
            this->setModelMatrix(glm::translate(this->getModelMatrix(), glm::vec3(-(this->size/2.0)+0.1, 0.0f, 0.0f)));
            this->bindMatrixData(window, scene, M_MODEL);
            glDrawElements(
                this->draw_method,
                2,
                GL_UNSIGNED_SHORT,
                (char *)NULL + (4));
            this->popModelMatrix(); // Pop 1

            // Return depth testing to what it was before we disabled it.
            glDepthFunc(old_depth_func);
        glUseProgram(0);
    glBindVertexArray(0);
}
