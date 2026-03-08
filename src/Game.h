// Game.h
#pragma once

#include "Scene.h"
#include <SDL_mixer.h>

class Game {
  public:
    static Game &getInstance() {
        static Game instance;
        return instance;
    }

    ~Game();
    void run();

    void init();
    void handleEvent(SDL_Event *event);
    void update(float deltaTime);
    void render();
    void clean();

    void changeScene(Scene *scene);

    SDL_Window *getWindow() { return window; }
    SDL_Renderer *getRenderer() { return renderer; }
    int getWindowWidth() { return windowWidth; }
    int getWindowHeight() { return windowHeight; }

  private:
    Game();
    // 删除拷贝与赋值构造函数
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    bool isRunning = true;
    Scene *currentScene = nullptr;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int windowWidth = 600;
    int windowHeight = 800;

    int FPS = 120;
    Uint32 frameTime;
    float deltaTime;
};
