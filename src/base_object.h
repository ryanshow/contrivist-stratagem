#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include <climits>
#include <vector>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "matrix.h"
#include "shader.h"

const GLuint RESTART_INDEX = UINT_MAX;

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
        void            bindMatrixData() const;
        virtual void    render(const Window & window, const Scene & scene);
        void            setVertices(Vertex* vert_list, int vert_count);
        void            setIndices(GLuint* pIndexList, int indexCount);

    protected:
        // Information about the model's vertices/indices
        std::vector<Vertex>     mVertexList;
        std::vector<GLuint>     mIndexList;
        GLuint *                mpBufferObjects;
        GLuint                  mVAO;
        static GLuint           msUBOBindingIndex;
        GLenum                  mDrawMethod;
};

#endif
