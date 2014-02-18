#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scene.h"
#include "grid.h"
#include "base_object.h"

//Initialize the static class variables
std::vector<Scene> Scene::scenes = std::vector<Scene>{};

Scene* Scene::addScene(int width, int height) {
    Scene::scenes.push_back(Scene(width, height));
    return &(Scene::scenes.back());
}

Scene::Scene(int width, int height) {
    // Initialize the view matrix, essentially the camera or "eye" orientation in space
    this->view_matrix = glm::mat4(1.0f);
    this->view_matrix *= glm::lookAt(
            glm::vec3(1.0f, 2.0f, 2.0f),    // The eye's position in 3d space
            glm::vec3(0.0f, 0.0f, 0.0f),    // What the eye is looking at
            glm::vec3(0.0f, 1.0f, 0.0f));    // The eye's orientation vector (which way is up)

    this->resizeCallback(width, height);

    this->clear_color_rgba[0] = 0.3;
    this->clear_color_rgba[1] = 0.3;
    this->clear_color_rgba[2] = 0.3;
    this->clear_color_rgba[3] = 1.0;

    this->grid = new Grid();
}

void Scene::render() {
    glClearColor(
            this->clear_color_rgba[0],
            this->clear_color_rgba[1],
            this->clear_color_rgba[2],
            this->clear_color_rgba[3]
    );

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Render the grid first so it doesn't render over scene objects
    if (this->grid != nullptr) {
        this->grid->render(this);
    }

    for (BaseObject* obj : this->objects) {
        obj->render(this);
    }
}

void Scene::resizeCallback(int width, int height) {
    // TODO: Clean this up for scenes taking up a small portion of the window

    this->width = width;
    this->height = height;

    // Update the projection matrix with the new ratio
    this->proj_matrix = glm::mat4(1.0f);
    this->proj_matrix *= glm::perspective(45.0f, float(width)/float(height), 0.1f, 100.0f);
}

void Scene::addObject(BaseObject* object) {
    this->objects.push_back(object);
}
