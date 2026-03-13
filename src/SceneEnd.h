// SceneEnd.h
#pragma once

#include "Scene.h"
#include <SDL_mixer.h>
#include <string>

class SceneEnd : public Scene {
  public:
    virtual void init();
    virtual void update(float deltaTime);
    virtual void render();
    virtual void clean();
    virtual void handleEvent(SDL_Event *event);

    // 处理文字
    void removeLastUTF8Char(std::string &str);

  private:
    void renderPhase1();
    void renderPhase2();

    bool isTyping = true;
    std::string name = "";
    float blinkTimer = 1.0f;

    Mix_Music *bgm;
};
