#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>

class MatrixStack {
    public:
    	MatrixStack();
    	MatrixStack(glm::mat4);

        glm::mat4 getMatrix();
        void setMatrix(glm::mat4);
        void pushMatrix();
        void popMatrix();

    private:
        std::vector<glm::mat4> mMatrixStack;
};

#endif
