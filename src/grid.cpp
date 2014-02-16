#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "format.h"

#include "grid.h"
#include "scene.h"

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

void Grid::render(Scene* scene) {
    // Make our vertex array active
    glBindVertexArray(this->vao);

    // Tell the renderer to use our shader program when rendering our object
    glUseProgram(this->shader->program_id);

    this->bindMatrixData(scene, M_PROJECTION|M_VIEW);

    for (int i=-1; i<=1; i+=2) {
        this->model_matrix = glm::translate(this->model_matrix, glm::vec3(-(this->size/2.0), 0.0f, 0.0f));
        for (int j=0; j<int(this->size/this->step)/2; j++) {
            this->model_matrix = glm::translate(this->model_matrix, glm::vec3(0.0f, 0.0f, i*this->step));
            this->bindMatrixData(scene, M_MODEL);
            // Render the vao on the screen
            glDrawElements(
                this->draw_method,
                2,
                GL_UNSIGNED_SHORT,
                (void*)0);
        }
        this->resetModelMatrix();
    }

    for (int i=-1; i<=1; i+=2) {
        this->model_matrix = glm::rotate(this->model_matrix, float(M_PI/2), glm::vec3(0.0f, -1.0f, 0.0f));
        this->model_matrix = glm::translate(this->model_matrix, glm::vec3(-(this->size/2.0), 0.0f, 0.0f));
        for (int j=0; j<int(this->size/this->step)/2; j++) {
            this->model_matrix = glm::translate(this->model_matrix, glm::vec3(0.0f, 0.0f, i*this->step));
            this->bindMatrixData(scene, M_MODEL);
            // Render the vao on the screen
            glDrawElements(
                this->draw_method,
                2,
                GL_UNSIGNED_SHORT,
                (void*)0);
        }
        this->resetModelMatrix();
    }

    this->resetModelMatrix();
    this->model_matrix = glm::translate(this->model_matrix, glm::vec3(-(this->size/2.0)+0.1, 0.0f, 0.0f));
    this->bindMatrixData(scene, M_MODEL);
    glDrawElements(
        this->draw_method,
        2,
        GL_UNSIGNED_SHORT,
        (char *)NULL + (4));

    this->resetModelMatrix();
    this->model_matrix = glm::rotate(this->model_matrix, float(M_PI/2), glm::vec3(0.0f, -1.0f, 0.0f));
    this->model_matrix = glm::translate(this->model_matrix, glm::vec3(-(this->size/2.0)+0.1, 0.0f, 0.0f));
    this->bindMatrixData(scene, M_MODEL);
    glDrawElements(
        this->draw_method,
        2,
        GL_UNSIGNED_SHORT,
        (char *)NULL + (4));

    this->resetModelMatrix();
}
