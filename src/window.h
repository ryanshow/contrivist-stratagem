#ifndef WINDOW_H
#define WINDOW_H

#include <map>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

class Grid;
class Scene;

class Window {
    public:
        Window() = default;
        Window(GLFWwindow *window);
        GLFWwindow* getWindow();
        void refresh();
        void addScene(Scene* scene);
        static Window* createWindow(const char* title, int width, int height);

    private:
        Scene* scene;
        GLFWwindow *window;

        static std::map<GLFWwindow*,Window> windows;
        static void resizeCallback(GLFWwindow* window, int width, int height);
};

#endif
