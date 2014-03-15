#ifndef SPRITE_H
#define SPRITE_H

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "base_object.h"

class Window;

class Sprite : public BaseObject {
    public:
        Sprite();
        void render(Window* window, Scene* scene);
    private:
        std::vector<unsigned char> mImageData;
        GLuint mTextureId;
        glm::uvec2 mTextureSize;
};

#endif
