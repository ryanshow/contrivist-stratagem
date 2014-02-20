#ifndef GRID_H
#define GRID_H

#include "base_object.h"

class Scene;
class Window;

class Grid : public BaseObject {
    public:
        Grid();
        void render(Window *window, Scene* scene);
        void setStep(int size, float step);

    private:
        int size;
        float step;
};

#endif
