// SceneMain.h
#pragma once

#include "Object.h"
#include "Scene.h"

class Game;

class SceneMain : public Scene {
  public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void handleEvent(SDL_Event *event) override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;

    void keyboardControl(float deltaTime);

  private:
    Game &game;
    Player player;
};
