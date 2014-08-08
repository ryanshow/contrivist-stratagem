#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <tuple>
#include <utility>

// OpenGL related headers
#include <GLFW/glfw3.h>

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

    // Initialize the camera's position in 3D space
    mCamFocus = glm::vec3(0.0f);
    mCamOrientation = glm::vec3(3.0f, M_PI/4, M_PI/2);

    glm::vec3 camPos;
    camPos.x = mCamOrientation.x * sin(mCamOrientation.y) * cos(mCamOrientation.z);
    camPos.y = mCamOrientation.x * sin(mCamOrientation.y) * sin(mCamOrientation.z);
    camPos.z = mCamOrientation.x * cos(mCamOrientation.y);

    // Initialize the view matrix, essentially the camera or "eye" orientation in space
    mViewMatrix = glm::mat4(1.0f);
    mViewMatrix *= glm::lookAt(
            camPos,                         // The eye's position in 3d space
            mCamFocus,                      // What the eye is looking at
            glm::vec3(0.0f, 1.0f, 0.0f));   // The eye's orientation vector (which way is up)

    mProjMatrix = glm::mat4(1.0f);
    mProjMatrix *= glm::perspective(45.0f, float(mSize.x)/float(mSize.y), 0.1f, 100.0f);

    /* Setup the proper callbacks */
    glfwSetFramebufferSizeCallback(mpWindow, this->resizeCallback);
    glfwSetScrollCallback(mpWindow, this->scrollCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(mpWindow);

    // Enable the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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

void Window::scrollCallback(GLFWwindow *glfwWindow, double xoffset, double yoffset) {
    Window* window = &(Window::msWindows[glfwWindow]);
    window->mCamOrientation += glm::vec3(yoffset, 0.0f, 0.0f);
}

void Window::camRotateCallback(GLFWwindow *glfwWindow, double xpos, double ypos) {
    Window* window = &(Window::msWindows[glfwWindow]);
    glm::dvec2 deltaPos = window->mCamMousePrevFrame - glm::dvec2(xpos, ypos);
    window->mCamOrientation.z += (deltaPos.x * 0.005);
    window->mCamOrientation.y += (deltaPos.y * 0.005);
    window->mCamMousePrevFrame = glm::vec2(xpos, ypos);
    std::cout << "mCamOrientation: " << window->mCamOrientation.x << " " << window->mCamOrientation.y << " " << window->mCamOrientation.z << std::endl;
}

void Window::addScene(Scene *pScene) {
    mpScene = pScene;
}

void Window::refresh() {
    glfwSetTime(0.0f);

    // Refresh the camera
    glm::vec3 camPos;
    camPos.z = mCamOrientation.x * sin(mCamOrientation.y) * cos(mCamOrientation.z);
    camPos.x = mCamOrientation.x * sin(mCamOrientation.y) * sin(mCamOrientation.z);
    camPos.y = mCamOrientation.x * cos(mCamOrientation.y);

    mViewMatrix = glm::mat4(1.0f);
    mViewMatrix *= glm::lookAt(
            camPos,                         // The eye's position in 3d space
            mCamFocus,                      // What the eye is looking at
            glm::vec3(0.0f, 1.0f, 0.0f));   // The eye's orientation vector (which way is up)

    glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mViewMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    mpScene->render(*this);

    /* Swap buffers, poll events */
    glfwSwapBuffers(mpWindow);
    glfwPollEvents();

    int middleMouseState = glfwGetMouseButton(mpWindow, GLFW_MOUSE_BUTTON_3);
    if (middleMouseState == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(mpWindow, &x, &y);
        mCamMousePrevFrame = glm::vec2(x, y);
        // FIXME: This is being called every time the mouse moves while the
        // middle mouse button is being held down. It should only be called once.
        glfwSetCursorPosCallback(mpWindow, this->camRotateCallback);
    } else if (middleMouseState == GLFW_RELEASE) {
        glfwSetCursorPosCallback(mpWindow, nullptr);
    }

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
