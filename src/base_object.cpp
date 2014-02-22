#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "format.h"

#include "base_object.h"
#include "scene.h"
#include "window.h"

BaseObject::BaseObject() {

    // Set up the default shader for the object
    ShaderTypeNameMap shader_type_names;
    shader_type_names[GL_VERTEX_SHADER] = "simple_shader.vert";
    shader_type_names[GL_FRAGMENT_SHADER] = "simple_shader.frag";
    //shader_type_names[GL_GEOMETRY_SHADER] = "simple_shader.geom";
    this->shader = Shader::getShader(shader_type_names);

    // Initialize the model transformation matrix
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::rotate(matrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, 0.0f));
    matrix = glm::scale(matrix, glm::vec3(1.0f, 1.0f, 1.0f));
    this->model_matrix_stack.push_back(matrix);

    // Set the default draw method
    this->draw_method = GL_LINES;

    // TODO: Combine these into a single array and generate them at once
    // Create the Vertex Array Object
    glGenVertexArrays(1, &this->vao);
    // Create the Vertex Buffer Object
    glGenBuffers(1, &this->vbo);
    // Create the Index Buffer Object
    glGenBuffers(1, &this->ibo);
    // Create the Uniform Buffer Object
    glGenBuffers(1, &this->ubo);

    // VAOs allow us to declare all of these attrib pointers while the VAO is
    // bound. This reduces clutter in the render member function.
    glBindVertexArray(this->vao);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
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
            // TODO: I should bind the last 3 textures here and access it as an array in the shader
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(
                3,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                (void*)offsetof(Vertex, s0));
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Bind the index buffer to the VAO, since that's what we'll use to render
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBindVertexArray(0);

    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + sizeof(glm::uvec2), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(this->getModelMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    this->ubo_binding_index = Window::createUniformBindingIndex();
    glUniformBlockBinding(
        this->shader->program_id,
        glGetUniformBlockIndex(this->shader->program_id, "Model"),
        this->ubo_binding_index);

}

void BaseObject::bindBufferData() {
    // Bind the vertex buffer data
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(Vertex)*this->vertex_list.size(),
            &(this->vertex_list)[0],
            GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind the Index buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(GLuint)*this->index_list.size(),
            &(this->index_list)[0],
            GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BaseObject::bindMatrixData(Window* window, Scene *scene, const unsigned char bind_mask) {
    if (bind_mask & M_PROJECTION || bind_mask & M_VIEW) {
        glUniformBlockBinding(
            this->shader->program_id,
            glGetUniformBlockIndex(this->shader->program_id, "Window"),
            window->ubo_binding_index);
        glBindBufferRange(GL_UNIFORM_BUFFER, window->ubo_binding_index, window->ubo, 0, sizeof(glm::mat4) * 2);
    }
    if (bind_mask & M_MODEL) {
        // FIXME: This should only occur when the model is transformed
        glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(this->getModelMatrix()));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // FIXME: It should not be necessary to bind this every frame.. find out why it's needed here.
        glUniformBlockBinding(
            this->shader->program_id,
            glGetUniformBlockIndex(this->shader->program_id, "Model"),
            this->ubo_binding_index);

        glBindBufferRange(GL_UNIFORM_BUFFER, this->ubo_binding_index, this->ubo, 0, sizeof(glm::mat4));
    }
}

void BaseObject::render(Window* window, Scene* scene) {

    // Make our vertex array active
    glBindVertexArray(this->vao);
        // Tell the renderer to use our shader program when rendering our object
        glUseProgram(this->shader->program_id);
            this->bindMatrixData(window, scene, M_PROJECTION|M_VIEW|M_MODEL);

            // Render the vao on the screen using "GL_LINE_LOOP"
            glDrawElements(
                this->draw_method,
                this->index_list.size(),
                GL_UNSIGNED_SHORT,
                (void*)0);
        glUseProgram(0);
    glBindVertexArray(0);
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

glm::mat4& BaseObject::getModelMatrix() {
    return this->model_matrix_stack.back();
}

void BaseObject::setModelMatrix(glm::mat4 matrix) {
    this->model_matrix_stack.back() = matrix;
}

void BaseObject::pushModelMatrix() {
    this->model_matrix_stack.push_back(this->model_matrix_stack.back());
}

void BaseObject::popModelMatrix() {
    this->model_matrix_stack.pop_back();
}
