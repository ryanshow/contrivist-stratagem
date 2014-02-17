
// OpenGL related headers
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

// Math Utilities
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

// String formatting library
#include <format.h>

// Version information for this application
#include "version.h"

// Other utility headers
#include "scene.h"
#include "grid.h"
#include "window.h"

double some_func(int i, double j) {
    return i * j;
}

// Prototypes
void errorCallback(int error, const char* description);

int main(int argc, char **argv)
{
    // Set the error callback early on so it can capture errors from creating a window
    glfwSetErrorCallback(errorCallback);

    // Initialize GLFW3
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Initialize Chaiscript engine
    chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());
    chai.add(chaiscript::fun(&some_func), "function");
    double d = chai.eval<double>("function(3, 4.75);");
    fmt::Print("Chai evaluated this: {0}\n") << d;

    const char* window_title = fmt::c_str(fmt::Format("Contravist Stratagem v{0}.{1} ({2})") << CS_VERSION_MAJOR << CS_VERSION_MINOR << GIT_SHA1);
    Window *window = Window::createWindow(window_title, 800, 600);

    // Enable VSync
    glfwSwapInterval(1);

    Scene* scene = Scene::addScene(800, 600);
    window->addScene(scene);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window->getWindow())) {
        window->refresh();
    }

    glfwDestroyWindow(window->getWindow());
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void errorCallback(int error, const char* description) {
    fmt::Print("GLFW ERROR: {0}\n") << description;
    glfwTerminate();
    exit(EXIT_FAILURE);
}
