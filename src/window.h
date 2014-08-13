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
        static GLuint msUBOBindingIndex;

        Window() = default;
        Window(GLFWwindow *window, glm::uvec2 size);
        GLFWwindow* getWindow();
        void refresh();
        void addScene(Scene* scene);
        glm::uvec2 getSize();
        void setSize(glm::uvec2 size);
        void updateViewMatrix();
        static Window* createWindow(const char* title, glm::uvec2 size);
        static GLuint createUniformBindingIndex();

    private:
        Scene* mpScene;
        GLFWwindow *mpWindow;
        glm::uvec2 mSize;

        glm::vec3 mCamFocus;        // The center of the camera's sphere of viewing
        glm::dvec3 mCamOrientation;  // Spherical coordinates (radius, inclination, azimuth)
        glm::dvec2 mCamMousePrevFrame;

        static std::map<GLFWwindow*,Window> msWindows;
        static GLuint msUBOBindingIndexCount;

        static void resizeCallback(GLFWwindow* window, int width, int height);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void camRotateCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif
