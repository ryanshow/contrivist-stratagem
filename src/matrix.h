#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

#include <glm/glm.hpp>

class MatrixStack {
    public:
        MatrixStack();
        MatrixStack(glm::mat4);

        glm::mat4   getMatrix() const;
        void        setMatrix(const glm::mat4 & matrix);
        void        pushMatrix();
        void        popMatrix();

    private:
        std::vector<glm::mat4> mMatrixStack;
};

#endif
