
#include <chrono>
#include <thread>
#include <utility>

// OpenGL related headers
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <format.h>

#include "scene.h"
#include "window.h"

#define MAX_FPS 25
#define MAX_FPS_INTERVAL (1/double(MAX_FPS))

//Initialize the static class variables
std::map<GLFWwindow*,Window> Window::windows = std::map<GLFWwindow*,Window>{};

Window::Window(GLFWwindow* window) {
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    this->window = window;

    /* Setup the proper callbacks */
    glfwSetFramebufferSizeCallback(this->window, this->resizeCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(this->window);

    // Enable the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

GLFWwindow* Window::getWindow() {
    return this->window;
}

void Window::resizeCallback(GLFWwindow *window, int width, int height) {
    // TODO: Support multiple scenes within a single window
    Window::windows[window].scene->resizeCallback(width, height);
}

void Window::addScene(Scene *scene) {
    this->scene = scene;
}

void Window::refresh() {
    glfwSetTime(0.0f);

    this->scene->render();

    /* Swap buffers, poll events */
    glfwSwapBuffers(this->window);
    glfwPollEvents();

    // Cap our framerate at MAX_FPS
    if (glfwGetTime() < MAX_FPS_INTERVAL) {
        std::this_thread::sleep_for(
            std::chrono::microseconds(
                int((MAX_FPS_INTERVAL - glfwGetTime()) * 1000000)
            )
        );
    }
}

Window* Window::createWindow(const char* title, int width, int height) {

    /* Request a OpenGL 3.3 context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(
            width, height,
            title,
            nullptr,
            nullptr);

    Window::windows.emplace(window, window);
    return &(Window::windows[window]);
}
