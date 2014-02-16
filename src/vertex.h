#ifndef VERTEX_H
#define VERTEX_H

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

typedef struct {
    GLfloat x, y, z;
    GLfloat nx, ny, nz;
    GLfloat r, g, b, a;
    GLfloat s0, t0;
    GLfloat s1, t1;
    GLfloat s2, t2;
} Vertex;

#endif
