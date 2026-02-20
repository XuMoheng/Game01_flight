// SceneMain.h
#pragma once

#include "Object.h"
#include "Scene.h"

class Game;

class SceneMain : public Scene {
  public:
    SceneMain();
    ~SceneMain();

    void update() override;
    void render() override;
    void handleEvent(SDL_Event *event) override;
    void init() override;
    void clean() override;

    void keyboardControl();

  private:
    Game &game;
    Player player;
};
