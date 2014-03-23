#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class BaseObject;
class Grid;
class Window;

class Scene {
    public:
        static std::vector<Scene>   scenes;
        glm::vec4                   mClearColor;

        Scene();

        static Scene *              addScene();
        void                        addObject(BaseObject* object);
        void                        render(const Window & window);
    private:
        Grid*                       mpGrid;
        std::vector<BaseObject*>    mObjectList;
};

#endif
