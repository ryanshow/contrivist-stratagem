#ifndef OCTREE_H
#define OCTREE_H

#include "base_object.h"

class Scene;
class Window;

class Octree : public BaseObject {
public:
    enum Octant {
        LDB,
        RDB,
        RUB,
        LUB,
        LDF,
        RDF,
        RUF,
        LUF
    };

    Octree(const glm::vec3 corner, const float size);
    Octree(Octree& parent, int octant);
    void render(const Window& window, const Scene& scene);
    void subdivide();

private:
    Octree* mpParent;
    std::vector<Octree> mChildren;
    glm::vec3 mCorner;
    float mEdgeSize;

    void setup();
};
#endif