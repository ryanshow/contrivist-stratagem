#ifndef SPRITE_H
#define SPRITE_H

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "base_object.h"

class Sprite : public BaseObject {
    public:
        Sprite();
        void render(Scene* scene);
    private:
        std::vector<unsigned char> image;
        GLuint textureId;
        glm::uvec2 textureSize;
};

#endif
