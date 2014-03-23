#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include <vector>

#include <GLFW/glfw3.h>

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

        Shader          *mpShader;
        MatrixStack     mModelMatrixStack;

        void            bindBufferData() const;
        void            bindMatrixData(const Window & window, const Scene & scene, const unsigned char bind_mask) const;
        virtual void    render(const Window & window, const Scene & scene);
        void            setVertices(Vertex* vert_list, int vert_count);
        void            setIndices(int* pIndexList, int indexCount);

    protected:
        // Information about the model's vertices/indices
        std::vector<Vertex>     mVertexList;
        std::vector<GLushort>   mIndexList;
        GLuint *                mpBufferObjects;
        GLuint                  mVAO;
        GLuint                  mUBOBindingIndex;
        GLenum                  mDrawMethod;
};

#endif
