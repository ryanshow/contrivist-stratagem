#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "base_object.h"
#include "grid.h"
#include "scene.h"
#include "window.h"

//Initialize the static class variables
std::vector<Scene> Scene::scenes = std::vector<Scene>{};

Scene* Scene::addScene() {
    Scene::scenes.push_back(Scene());
    return &(Scene::scenes.back());
}

Scene::Scene() {
    this->clear_color_rgba[0] = 0.3;
    this->clear_color_rgba[1] = 0.3;
    this->clear_color_rgba[2] = 0.3;
    this->clear_color_rgba[3] = 1.0;

    this->grid = new Grid();
}

void Scene::render(Window* window) {
    glClearColor(
            this->clear_color_rgba[0],
            this->clear_color_rgba[1],
            this->clear_color_rgba[2],
            this->clear_color_rgba[3]
    );

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Render the grid first so it doesn't render over scene objects
    if (this->grid != nullptr) {
        this->grid->render(window, this);
    }

    for (BaseObject* obj : this->objects) {
        obj->render(window, this);
    }
}

void Scene::addObject(BaseObject* object) {
    this->objects.push_back(object);
}
