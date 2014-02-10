
#include <chrono>
#include <thread>

// OpenGL related headers
#include <GL/glew.h>
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
#include "shader.h"
#include "vertex.h"

#define MAX_FPS 60
#define MAX_FPS_INTERVAL (1/double(MAX_FPS))

// Prototypes
void framebufferSizeCallback(GLFWwindow*, int, int);
void errorCallback(int error, const char* description);

// Local namespace
namespace {
    GLFWwindow *window;

    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
    glm::mat4 model_matrix;
}

int main(int argc, char **argv)
{
    /* Set the error callback early on so it can capture errors from creating a window */
    glfwSetErrorCallback(errorCallback);

    /* Initialize the library */
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

    /* Initialize GLEW */
    GLenum glew_err = glewInit();
    if (glew_err != GLEW_OK) {
        fmt::Print("GLEW ERROR: {0}\n") << reinterpret_cast<const char*>(glewGetErrorString(glew_err));
    }

    ShaderTypeNameMap shader_type_names;
    shader_type_names[GL_VERTEX_SHADER] = "simple_shader.vert";
    shader_type_names[GL_FRAGMENT_SHADER] = "simple_shader.frag";
    Shader::getShader(shader_type_names);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwSetTime(0.0f);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        /* Swap buffers, poll events */
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Cap our framerate at MAX_FPS
        if (glfwGetTime() < MAX_FPS_INTERVAL) {
            std::this_thread::sleep_for(std::chrono::microseconds(int((MAX_FPS_INTERVAL - glfwGetTime()) * 1000000)));
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void init_gl(GLFWwindow* window) {
    // Enable the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set the clear color
    glClearColor(0.1, 0.1, 0.1, 1.0);

    // Initialize the model transformation matrix
    // FIXME: Split this out, eventually... or rename it to "scene"
    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::rotate(model_matrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
    model_matrix = glm::scale(model_matrix, glm::vec3(1.0f, 1.0f, 1.0f));

    // Initialize the projection matrix (Gives the world a perspective feel, rather than orthographic)
    projection_matrix = glm::mat4(1.0f);
    projection_matrix *= glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);

    // Initialize the view matrix, essentially the camera or "eye" orientation in space
    view_matrix = glm::mat4(1.0f);
    view_matrix *= glm::lookAt(
            glm::vec3(1.0f, 3.0f, 5.0f),    // The eye's position in 3d space
            glm::vec3(0.0f, 0.0f, 0.0f),    // What the eye is looking at
            glm::vec3(0.0f, 1.0f, 0.0f));    // The eye's orientation vector (which way is up)
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {

    // Set the rendering viewport in the window
    glViewport(0, 0, width, height);

    // Update the projection matrix with the new ratio
    projection_matrix = glm::mat4(1.0f);
    projection_matrix *= glm::perspective(45.0f, float(width)/float(height), 0.1f, 100.0f);

}

void errorCallback(int error, const char* description) {
    fmt::Print("GLFW ERROR: {0}\n") << description;
    glfwTerminate();
    exit(EXIT_FAILURE);
}


/*
class Grid: public BasicObject {
    public:


}

void drawGrid() {
    Vertex v0, v1;
    v0.x = -1.0f; v0.y =  0.0f; v0.z =  0.0f; vertex_list.push_back(v0);
    v1.x =  1.0f; v1.y =  0.0f; v1.z =  0.0f; vertex_list.push_back(v1);

    index_list.push_back(0); index_list.push_back(1);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vert_list.size(), &(vert_list)[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*index_list.size(), &(index_list)[0], GL_STATIC_DRAW);


}
*/
