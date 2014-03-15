#ifndef SCENE_H
#define SCENE_H

#include <vector>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class BaseObject;
class Grid;
class Window;

class Scene {
    public:
        static std::vector<Scene> scenes;

        glm::vec4 mClearColor;

        static Scene* addScene();

        Scene();
        void addObject(BaseObject* object);
        void render(Window* window);
    private:
        Grid* mpGrid;
        std::vector<BaseObject*> mObjectList;
};

#endif
