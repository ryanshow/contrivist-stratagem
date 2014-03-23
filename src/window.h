#ifndef WINDOW_H
#define WINDOW_H

#include <map>

#include <GLFW/glfw3.h>

class Grid;
class Scene;

class Window {
    public:
        glm::mat4 mViewMatrix;
        glm::mat4 mProjMatrix;
        GLuint mUBO;
        GLuint mUBOBindingIndex;

        Window() = default;
        Window(GLFWwindow *window, glm::uvec2 size);
        GLFWwindow* getWindow();
        void refresh();
        void addScene(Scene* scene);
        glm::uvec2 getSize();
        void setSize(glm::uvec2 size);
        static Window* createWindow(const char* title, glm::uvec2 size);
        static GLuint createUniformBindingIndex();

    private:
        Scene* mpScene;
        GLFWwindow *mpWindow;
        glm::uvec2 mSize;

        static std::map<GLFWwindow*,Window> msWindows;
        static GLuint msUBOBindingIndexCount;

        static void resizeCallback(GLFWwindow* window, int width, int height);
};

#endif
