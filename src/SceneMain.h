// SceneMain.h
#pragma once

#include "Object.h"
#include "Scene.h"
#include <list>

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

    void shootPlayer();
    void updatePlayerProjectile(float deltaTime);
    void renderPlayerProjectiles();

  private:
    Game &game;
    Player player;
    ProjectilePlayer projectilePlayerTemplate;
    std::list<ProjectilePlayer *> projectilesPlayer;
};
