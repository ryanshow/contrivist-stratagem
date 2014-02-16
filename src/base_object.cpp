#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "format.h"

#include "base_object.h"
#include "scene.h"

BaseObject::BaseObject() {

    fmt::Print("BaseObject constructor\n");

    // Set up the default shader for the object
    ShaderTypeNameMap shader_type_names;
    shader_type_names[GL_VERTEX_SHADER] = "simple_shader.vert";
    shader_type_names[GL_FRAGMENT_SHADER] = "simple_shader.frag";
    this->shader = Shader::getShader(shader_type_names);

    // Initialize the model transformation matrix
    this->resetModelMatrix();

    // Set the default draw method
    this->draw_method = GL_LINES;

    // Create the Vertex Array Object
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    // Create the Vertex Buffer Object
    glGenBuffers(1, &this->vbo_vertices);

    // Create the Index Buffer Object
    glGenBuffers(1, &this->vbo_indices);

    // Provide mechanisms for the shaders to access vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices);

    // Position (XYZ)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, x));

    // Normal (XYZ)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, nx));

    // Color (RGBA)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, r));

    // Texture (ST)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, s0));
}

void BaseObject::bindBufferData() {
    // Bind the vertex buffer data
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(Vertex)*this->vertex_list.size(),
        &(this->vertex_list)[0],
        GL_STATIC_DRAW);

    // Bind the Index buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_indices);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint)*this->index_list.size(),
        &(this->index_list)[0],
        GL_STATIC_DRAW);
}

void BaseObject::resetModelMatrix() {
    this->model_matrix = glm::mat4(1.0f);
    this->model_matrix = glm::rotate(this->model_matrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    this->model_matrix = glm::translate(this->model_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
    this->model_matrix = glm::scale(this->model_matrix, glm::vec3(1.0f, 1.0f, 1.0f));
}

void BaseObject::bindMatrixData(Scene *scene, const unsigned char bind_mask) {
    if (bind_mask & M_PROJECTION) {
        // Bind the "proj_matrix" variable in our C++ program to the "Projection" variable in the shader
        glUniformMatrix4fv(
            glGetUniformLocation(this->shader->program_id, "Projection"),
            1,
            false,
            glm::value_ptr(scene->proj_matrix));
    }
    if (bind_mask & M_VIEW) {
        // Bind the "view_matrix" variable in our C++ program to the "View" variable in the shader
        glUniformMatrix4fv(
            glGetUniformLocation(this->shader->program_id, "View"),
            1,
            false,
            glm::value_ptr(scene->view_matrix));
    }
    if (bind_mask & M_MODEL) {
        // Bind the "model_matrix" variable in our C++ program to the "Model" variable in the shader
        glUniformMatrix4fv(
            glGetUniformLocation(this->shader->program_id, "Model"),
            1,
            false,
            glm::value_ptr(this->model_matrix));
    }
}
void BaseObject::render(Scene* scene) {

    // Make our vertex array active
    glBindVertexArray(this->vao);

    // Tell the renderer to use our shader program when rendering our object
    glUseProgram(this->shader->program_id);

    this->bindMatrixData(scene, M_PROJECTION|M_VIEW|M_MODEL);

    // Render the vao on the screen using "GL_LINE_LOOP"
    glDrawElements(
        this->draw_method,
        this->index_list.size(),
        GL_UNSIGNED_SHORT,
        (void*)0);
}

void BaseObject::setVertices(Vertex* vert_list, int vert_count) {
    this->vertex_list.clear();
    for (int i=0; i<vert_count; i++) {
        this->vertex_list.push_back(vert_list[i]);
    }
}

void BaseObject::setIndices(int* index_list, int index_count) {
    this->index_list.clear();
    for (int i=0; i<index_count; i++) {
        this->index_list.push_back(index_list[i]);
    }
}
