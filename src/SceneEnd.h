// SceneEnd.h
#pragma once

#include "Scene.h"

class SceneEnd : public Scene {
    virtual void init();
    virtual void update(float deltaTime);
    virtual void render();
    virtual void clean();
    virtual void handleEvent(SDL_Event *event);
};
