#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include <vector>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "vertex.h"

#define M_PROJECTION 1
#define M_VIEW       2
#define M_MODEL      4

class Scene;

class BaseObject {
    public:
        // The model's shader information
        Shader *shader;

        BaseObject();

        void bindBufferData();
        void bindMatrixData(Scene *scene, const unsigned char bind_mask);
        virtual void render(Scene* scene);
        void setVertices(Vertex* vert_list, int vert_count);
        void setIndices(int* index_list, int index_count);

        glm::mat4& getModelMatrix();
        void setModelMatrix(glm::mat4 matrix);
        void pushModelMatrix();
        void popModelMatrix();

    protected:
        // The model's transformation matrix
        // TODO: convert this into a vector and use a push/pop model instead
        glm::mat4 model_matrix;
        std::vector<glm::mat4> model_matrix_stack;

        // Information about the model's vertices/indices
        std::vector<Vertex> vertex_list;
        std::vector<GLushort> index_list;
        GLuint vao, vbo_vertices, vbo_indices;
        GLenum draw_method;
};

#endif
