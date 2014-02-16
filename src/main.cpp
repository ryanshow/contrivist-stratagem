
#include <chrono>
#include <thread>

// OpenGL related headers
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

// Math Utilities
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// String formatting library
#include <format.h>

// Version information for this application
#include "version.h"

// Other utility headers
#include "scene.h"
#include "grid.h"

#define MAX_FPS 6
#define MAX_FPS_INTERVAL (1/double(MAX_FPS))

// Prototypes
void framebufferSizeCallback(GLFWwindow*, int, int);
void errorCallback(int error, const char* description);
void init_gl();

// Local namespace
namespace {
    GLFWwindow *window;

    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
}

int main(int argc, char **argv)
{
    /* Set the error callback early on so it can capture errors from creating a window */
    glfwSetErrorCallback(errorCallback);

    /* Initialize glfw */
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    /* Request a OpenGL 3.3 context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(
            800, 600,
            fmt::c_str(fmt::Format("Contravist Stratagem v{0}.{1} ({2})") << CS_VERSION_MAJOR << CS_VERSION_MINOR << GIT_SHA1),
            nullptr,
            nullptr);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* Setup the proper callbacks */
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Enable VSync */
    glfwSwapInterval(1);

    init_gl();

    Grid* obj = new Grid();

    Scene* scene = Scene::addScene(800, 600);
    scene->addObject(obj);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwSetTime(0.0f);

        for (Scene scene : Scene::scenes) {
            scene.render();
        }

        /* Swap buffers, poll events */
        glfwSwapBuffers(window);
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

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void init_gl() {
    // Enable the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {

    // Set the rendering viewport in the window
    glViewport(0, 0, width, height);

    for (Scene scene : Scene::scenes) {
        scene.resizeCallback(width, height);
    }
}

void errorCallback(int error, const char* description) {
    fmt::Print("GLFW ERROR: {0}\n") << description;
    glfwTerminate();
    exit(EXIT_FAILURE);
}
