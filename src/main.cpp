#include <GLFW/glfw3.h>
#include <format.h>

#include "version.h"

int main(int argc, char **argv)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(
            800, 600,
            fmt::c_str(fmt::Format("Contravist Stratagem v{0}.{1} ({2})") << CS_VERSION_MAJOR << CS_VERSION_MINOR << GIT_SHA1),
            nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
} 
