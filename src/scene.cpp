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
    mClearColor = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);

    mpGrid = new Grid();
}

void Scene::render(Window* window) {
    glClearColor(
        mClearColor.r,
        mClearColor.g,
        mClearColor.b,
        mClearColor.a
    );

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Render the grid first so it doesn't render over scene objects
    if (mpGrid != nullptr) {
        mpGrid->render(window, this);
    }

    for (BaseObject* obj : mObjectList) {
        obj->render(window, this);
    }
}

void Scene::addObject(BaseObject* object) {
    mObjectList.push_back(object);
}
