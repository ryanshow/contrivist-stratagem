#ifndef WINDOW_H
#define WINDOW_H

#include <map>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

class Grid;
class Scene;

class Window {
    public:
        glm::mat4 view_matrix;
        glm::mat4 proj_matrix;
        GLuint ubo;
        GLuint ubo_binding_index;

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
        Scene* scene;
        GLFWwindow *window;
        glm::uvec2 size;

        static std::map<GLFWwindow*,Window> windows;
        static GLuint ubo_binding_index_count;
        static void resizeCallback(GLFWwindow* window, int width, int height);
};

#endif
