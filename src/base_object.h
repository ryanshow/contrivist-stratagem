#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include <GLFW/glfw3.h>

#include "shader.h"

class BaseObject {
    public:
        //std::vector<Vertex> vert_list;
        //std::vector<GLushort> index_list;
        //GLuint vao, vbo_vertices, vbo_indices;
        BaseObject();
        Shader *shader;
};

#endif
