
#include <chrono>
#include <thread>
#include <tuple>
#include <utility>

// OpenGL related headers
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <format.h>

#include "scene.h"
#include "window.h"

#define MAX_FPS 25
#define MAX_FPS_INTERVAL (1/double(MAX_FPS))

//Initialize the static class variables
std::map<GLFWwindow*,Window> Window::msWindows = std::map<GLFWwindow*,Window>{};
GLuint Window::msUBOBindingIndexCount = 0;

Window::Window(GLFWwindow* window, glm::uvec2 size) {
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    mpWindow = window;
    mSize = size;

    /* Setup the proper callbacks */
    glfwSetFramebufferSizeCallback(mpWindow, this->resizeCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(mpWindow);

    // Enable the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Initialize the view matrix, essentially the camera or "eye" orientation in space
    mViewMatrix = glm::mat4(1.0f);
    mViewMatrix *= glm::lookAt(
            glm::vec3(1.0f, 2.0f, 2.0f),    // The eye's position in 3d space
            glm::vec3(0.0f, 0.0f, 0.0f),    // What the eye is looking at
            glm::vec3(0.0f, 1.0f, 0.0f));    // The eye's orientation vector (which way is up)
    mProjMatrix = glm::mat4(1.0f);
    mProjMatrix *= glm::perspective(45.0f, float(size.x)/float(size.y), 0.1f, 100.0f);

    // Create the uniform buffer object that will hold this window's variables
    // that we need to pass to the shader.
    glGenBuffers(1, &mUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mProjMatrix));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mViewMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    mUBOBindingIndex = Window::createUniformBindingIndex();
}

GLFWwindow* Window::getWindow() {
    return mpWindow;
}

void Window::resizeCallback(GLFWwindow *glfwWindow, int width, int height) {
    Window* window = &(Window::msWindows[glfwWindow]);
    window->mSize.x = width;
    window->mSize.y = height;

    window->mProjMatrix = glm::mat4(1.0f);
    window->mProjMatrix *= glm::perspective(45.0f, float(width)/float(height), 0.1f, 100.0f);

    glBindBuffer(GL_UNIFORM_BUFFER, window->mUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(window->mProjMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Window::addScene(Scene *pScene) {
    mpScene = pScene;
}

void Window::refresh() {
    glfwSetTime(0.0f);

    mpScene->render(this);

    /* Swap buffers, poll events */
    glfwSwapBuffers(mpWindow);
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

Window* Window::createWindow(const char* title, glm::uvec2 size) {

    /* Request a OpenGL 3.3 context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(
            size.x, size.y,
            title,
            nullptr,
            nullptr);

    Window::msWindows.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(window),
            std::forward_as_tuple(window, size)
    );
    return &(Window::msWindows[window]);
}

glm::uvec2 Window::getSize() {
    return mSize;
}

void Window::setSize(glm::uvec2 size) {
    glfwSetWindowSize(mpWindow, size.x, size.y);
    // The class member size variables will be set in the callback
}

GLuint Window::createUniformBindingIndex() {
    return Window::msUBOBindingIndexCount++;
}
