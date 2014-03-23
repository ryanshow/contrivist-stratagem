#include "matrix.h"

MatrixStack::MatrixStack() {
    mMatrixStack.push_back(glm::mat4(1.0f));
}

MatrixStack::MatrixStack(glm::mat4 initMatrix) {
    mMatrixStack.push_back(initMatrix);
}

glm::mat4 MatrixStack::getMatrix() const {
    return mMatrixStack.back();
}

void MatrixStack::setMatrix(const glm::mat4 & matrix) {
    mMatrixStack.back() = matrix;
}

void MatrixStack::pushMatrix() {
    mMatrixStack.push_back(mMatrixStack.back());
}

void MatrixStack::popMatrix() {
    // Require the stack to have more than 1 matrix before we pop the top off
    assert(mMatrixStack.size() > 1);
    mMatrixStack.pop_back();
}
