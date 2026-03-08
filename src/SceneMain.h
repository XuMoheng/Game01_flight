// SceneMain.h
#pragma once

#include "Object.h"
#include "Scene.h"
#include <SDL_mixer.h>
#include <list>
#include <map>
#include <random>

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

    // 射击
    void shootPlayer();
    void updatePlayerProjectiles(float deltaTime);
    void renderPlayerProjectiles();

    // 敌机
    void spawEnemy();
    void updateEnemies(float deltaTime);
    void renderEnemies();
    void enemyExplode(Enemy *enemy);

    // 敌机子弹
    void shootEnemy(Enemy *enemy);
    SDL_FPoint getDirection(Enemy *enemy);
    void updateEnemyProjectiles(float deltaTime);
    void renderEnemyProjectiles();

    // 玩家
    void updatePlayer(float deltaTime);

    // 爆炸动画
    void updateExplosions(float deltaTime);
    void renderExplosions();

    // 道具
    void dropItem(Enemy *enemy);
    void playerGetItem(Item *item);
    void updateItems(float deltaTime);
    void renderItems();

  private:
    Game &game;
    Player player;
    ProjectilePlayer projectilePlayerTemplate;
    std::list<ProjectilePlayer *> projectilesPlayer;

    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    Enemy enemyTemplate;
    std::list<Enemy *> enemies;

    ProjectileEnemy projectileEnemyTemplate;
    std::list<ProjectileEnemy *> projectilesEnemy;

    Explosion explosionTemplate;
    std::list<Explosion *> explosions;

    Item itemLifeTemplate;
    std::list<Item *> items;

    Mix_Music *bgm;
    std::map<std::string, Mix_Chunk *> sounds;

    bool isDead = false;
};
