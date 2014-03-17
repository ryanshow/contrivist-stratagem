#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include <vector>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum {
    M_PROJECTION = 1,
    M_VIEW = 2,
    M_MODEL = 4
};

#include "matrix.h"
#include "shader.h"

class Scene;
class Window;

class BaseObject {
    protected:
        typedef struct {
            glm::vec3 pos;
            glm::vec3 nor;
            glm::vec4 col;
            glm::vec2 tx0;
            glm::vec2 tx1;
            glm::vec2 tx2;
        } Vertex;

        enum {
            VERTEX,
            INDEX,
            UNIFORM
        };

    public:
        BaseObject();

        // The model's shader information
        Shader *mpShader;
        MatrixStack mModelMatrixStack;

        void bindBufferData();
        void bindMatrixData(Window* window, Scene *scene, const unsigned char bind_mask);
        virtual void render(Window* window, Scene* scene);
        void setVertices(Vertex* vert_list, int vert_count);
        void setIndices(int* pIndexList, int indexCount);

    protected:
        // Information about the model's vertices/indices
        std::vector<Vertex> mVertexList;
        std::vector<GLushort> mIndexList;
        GLuint *mpBufferObjects;
        GLuint mVAO, mUBOBindingIndex;
        GLenum mDrawMethod;
};

#endif
