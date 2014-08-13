#include <chrono>
#include <iostream>
#include <random>

#include <GLFW/glfw3.h>

#include "octree.h"

Octree::Octree(const glm::vec3 corner, const float size) {
    mpParent = nullptr;

    mCorner = corner;
    mEdgeSize = size;

    setup();

    subdivide();
}

Octree::Octree(Octree& parent, int octant) {
    mpParent = &parent;
    mEdgeSize = parent.mEdgeSize/2;
    switch (octant) {
        case LDB:
            mCorner = parent.mCorner;
            break;
        case RDB:
            mCorner = parent.mCorner + glm::vec3(mEdgeSize, 0.0f, 0.0f);
            break;
        case RUB:
            mCorner = parent.mCorner + glm::vec3(mEdgeSize, mEdgeSize, 0.0f);
            break;
        case LUB:
            mCorner = parent.mCorner + glm::vec3(0.0f, mEdgeSize, 0.0f);
            break;
        case LDF:
            mCorner = parent.mCorner + glm::vec3(0.0f, 0.0f, mEdgeSize);
            break;
        case RDF:
            mCorner = parent.mCorner + glm::vec3(mEdgeSize, 0.0f, mEdgeSize);
            break;
        case RUF:
            mCorner = parent.mCorner + glm::vec3(mEdgeSize, mEdgeSize, mEdgeSize);
            break;
        case LUF:
            mCorner = parent.mCorner + glm::vec3(0.0f, mEdgeSize, mEdgeSize);
            break;
    }

    setup();

    static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::minstd_rand generator (seed);
    static std::uniform_int_distribution<int> distribution(1,5);
    int dice_roll = distribution(generator);
    if (mEdgeSize > 0.0625 && (dice_roll * mEdgeSize) >= 1) {
        subdivide();
    }

}

void Octree::render(const Window& window, const Scene& scene) {
    if (mEdgeSize == 0.125 && mChildren.size() > 0) {
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, mpBufferObjects[UNIFORM], 0, sizeof(glm::mat4)+sizeof(glm::uvec2));
        // Make our vertex array active
        glBindVertexArray(mVAO);
            // Tell the renderer to use our shader program when rendering our object
            glUseProgram(mpShader->mProgramId);

                // Render the vao on the screen using "GL_LINE_LOOP"
                glDrawElements(
                    mDrawMethod,
                    mIndexList.size(),
                    GL_UNSIGNED_SHORT,
                    (void*)0);

            glUseProgram(0);
        glBindVertexArray(0);
    }

    for (Octree octree : mChildren) {
        octree.render(window, scene);
    }
}

void Octree::setup() {

    ShaderTypeNameMap shader_type_names;
    shader_type_names[GL_VERTEX_SHADER] = "simple_shader.vert";
    shader_type_names[GL_GEOMETRY_SHADER] = "wireframe_shader.geom";
    shader_type_names[GL_FRAGMENT_SHADER] = "wireframe_shader.frag";
    mpShader = Shader::getShader(shader_type_names);

    mDrawMethod = GL_TRIANGLE_STRIP;

    Vertex v[8] {};

    for (int i=0; i<8; i++) {
        v[i].col = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    v[0].pos = mCorner;
    v[1].pos = mCorner + glm::vec3(mEdgeSize, 0.0f, 0.0f);
    v[2].pos = mCorner + glm::vec3(mEdgeSize, mEdgeSize, 0.0f);
    v[3].pos = mCorner + glm::vec3(0.0f, mEdgeSize, 0.0f);

    v[4].pos = mCorner + glm::vec3(0.0f, 0.0f, mEdgeSize);
    v[5].pos = mCorner + glm::vec3(mEdgeSize, 0.0f, mEdgeSize);
    v[6].pos = mCorner + glm::vec3(mEdgeSize, mEdgeSize, mEdgeSize);
    v[7].pos = mCorner + glm::vec3(0.0f, mEdgeSize, mEdgeSize);

    int i[17] {1, 0, 2, 3, 6, 7, 5, 4, 8,
               6, 2, 5, 1, 4, 0, 7, 3};

    setVertices(v, 8);
    setIndices(i, 17);
    bindBufferData();
}

void Octree::subdivide() {
    for (int i=0; i<8; i++) {
        Octree* octree = new Octree(*this, i);
        mChildren.push_back(*octree);
    }
}
