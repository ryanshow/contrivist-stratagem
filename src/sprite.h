#ifndef SPRITE_H
#define SPRITE_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "base_object.h"

class Window;

struct SpriteAnim {
  glm::uvec2 frameStartPos;
  unsigned short frameStride;
  unsigned short frameCount;
};

class Sprite : public BaseObject {
    public:
        Sprite(const glm::vec3 pos);
        void render(const Window & window, const Scene & scene);
    private:
        std::vector<unsigned char> mTextureData;
        GLuint mTextureId;
        glm::uvec2 mTextureSize;
        std::map<std::string, SpriteAnim> mAnimationMap;
};

#endif
