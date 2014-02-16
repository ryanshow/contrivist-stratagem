#ifndef SCENE_H
#define SCENE_H

#include <vector>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class BaseObject;

class Scene {
    public:
        static std::vector<Scene> scenes;

        glm::mat4 view_matrix;
        glm::mat4 proj_matrix;
        int width, height;
        GLfloat clear_color_rgba[5];

        static Scene* addScene(int width, int height);

        Scene(int width, int height);
        void addObject(BaseObject* object);
        void render();
        void resizeCallback(int width, int height);
    private:
        std::vector<BaseObject*> objects;
};

#endif
