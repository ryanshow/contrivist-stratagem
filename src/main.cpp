// OpenGL related headers
#include <GLFW/glfw3.h>

// Math Utilities
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// String formatting library
#include <format.h>

// Version information for this application
#include "version.h"

// Other utility headers
#include "grid.h"
#include "octree.h"
#include "scene.h"
#include "sprite.h"
#include "window.h"

void errorCallback(int error, const char* description) {
    fmt::Print("GLFW ERROR: {0}\n") << description;
    glfwTerminate();
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    // Set the error callback early on so it can capture errors from creating a window
    glfwSetErrorCallback(errorCallback);

    // Initialize GLFW3
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    const char* window_title = fmt::c_str(fmt::Format("Contravist Stratagem v{0}.{1} ({2})") << CS_VERSION_MAJOR << CS_VERSION_MINOR << GIT_SHA1);
    Window *window = Window::createWindow(window_title, glm::uvec2(800, 600));

    // Enable VSync
    glfwSwapInterval(1);

    Scene* scene = Scene::addScene();
    scene->addObject(new Sprite(glm::vec3(0.0, 1.0, 0.0)));
    scene->addObject(new Octree(glm::vec3(-1.0, -1.0, -1.0), 2.0));
    window->addScene(scene);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window->getWindow())) {
        window->refresh();
    }

    glfwDestroyWindow(window->getWindow());
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
