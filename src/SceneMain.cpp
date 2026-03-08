// SceneMain.cpp
#include "SceneMain.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_scancode.h>
#include <random>

SceneMain::SceneMain() : game(Game::getInstance()) {}

SceneMain::~SceneMain() {}

void SceneMain::init() {
    // 初始化随机数生成器
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

    // 初始化玩家
    player.texture =
        IMG_LoadTexture(game.getRenderer(), "../assets/image/SpaceShip.png");
    if (player.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "Failed to load player texture: %s", SDL_GetError());
    }
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;

    // 初始化玩家的子弹模板
    projectilePlayerTemplate.texture =
        IMG_LoadTexture(game.getRenderer(), "../assets/image/laser-1.png");
    SDL_QueryTexture(projectilePlayerTemplate.texture, NULL, NULL,
                     &projectilePlayerTemplate.width,
                     &projectilePlayerTemplate.height);
    projectilePlayerTemplate.width /= 4;
    projectilePlayerTemplate.height /= 4;

    // 初始化敌机
    enemyTemplate.texture =
        IMG_LoadTexture(game.getRenderer(), "../assets/image/insect-2.png");
    SDL_QueryTexture(enemyTemplate.texture, NULL, NULL, &enemyTemplate.width,
                     &enemyTemplate.height);
    enemyTemplate.width /= 4;
    enemyTemplate.height /= 4;

    // 初始化敌机子弹模板
    projectileEnemyTemplate.texture =
        IMG_LoadTexture(game.getRenderer(), "../assets/image/bullet-1.png");
    SDL_QueryTexture(projectileEnemyTemplate.texture, NULL, NULL,
                     &projectileEnemyTemplate.width,
                     &projectileEnemyTemplate.height);
    projectileEnemyTemplate.width /= 4;
    projectileEnemyTemplate.height /= 4;
}

void SceneMain::handleEvent([[maybe_unused]] SDL_Event *event) {}

void SceneMain::update(float deltaTime) {
    keyboardControl(deltaTime);
    updatePlayerProjectiles(deltaTime);
    updateEnemyProjectiles(deltaTime);
    spawEnemy();
    updateEnemies(deltaTime);
    updatePlayer(deltaTime);
}

void SceneMain::render() {
    // 渲染玩家子弹
    renderPlayerProjectiles();
    // 渲染敌机子弹
    renderEnemyProjectiles();
    // 渲染玩家
    if (!isDead) {
        SDL_Rect playerRect = {static_cast<int>(player.position.x),
                               static_cast<int>(player.position.y),
                               player.width, player.height};
        SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
    }
    // 渲染敌机
    renderEnemies();
}

void SceneMain::clean() {
    // 清理子弹
    for (auto &projectile : projectilesPlayer) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilesPlayer.clear();

    // 清理敌机
    for (auto &enemy : enemies) {
        if (enemy != nullptr) {
            delete enemy;
        }
    }
    enemies.clear();

    // 清理敌机子弹
    for (auto &projectile : projectilesEnemy) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilesEnemy.clear();

    // 清理纹理
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
    }
    if (projectilePlayerTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
    }
    if (enemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(enemyTemplate.texture);
    }
    if (projectileEnemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
    }
}

void SceneMain::keyboardControl(float deltaTime) {
    auto keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_W]) {
        player.position.y -= player.speed * deltaTime;
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        player.position.y += player.speed * deltaTime;
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        player.position.x -= player.speed * deltaTime;
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        player.position.x += player.speed * deltaTime;
    }

    // 限制飞机的移动范围
    if (player.position.x < 0) {
        player.position.x = 0;
    }
    if (player.position.x > game.getWindowWidth() - player.width) {
        player.position.x = game.getWindowWidth() - player.width;
    }
    if (player.position.y < 0) {
        player.position.y = 0;
    }
    if (player.position.y > game.getWindowHeight() - player.height) {
        player.position.y = game.getWindowHeight() - player.height;
    }

    // 控制子弹发射
    if (keyboardState[SDL_SCANCODE_J]) {
        auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastShootTime > player.coolDown && !isDead) {
            shootPlayer();
            player.lastShootTime = currentTime;
        }
    }
}

void SceneMain::shootPlayer() {
    auto projectile = new ProjectilePlayer(projectilePlayerTemplate);
    // 子弹初始位置在飞机顶部中心
    projectile->position.x =
        player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y = player.position.y;
    projectilesPlayer.push_back(projectile);
}

void SceneMain::updatePlayerProjectiles(float deltaTime) {
    int margin = 32; // 子弹超出屏幕外边界的距离
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();) {
        auto projectile = *it;
        projectile->position.y -= projectile->speed * deltaTime;
        if (projectile->position.y + margin < 0) {
            delete projectile;
            it = projectilesPlayer.erase(it);
        } else {
            bool hit = false;
            for (auto enemy : enemies) {
                SDL_Rect enemyRect = {static_cast<int>(enemy->position.x),
                                      static_cast<int>(enemy->position.y),
                                      enemy->width, enemy->height};
                SDL_Rect projectileRect = {
                    static_cast<int>(projectile->position.x),
                    static_cast<int>(projectile->position.y), projectile->width,
                    projectile->height};
                if (SDL_HasIntersection(&enemyRect, &projectileRect)) {
                    enemy->currentHealth -= projectile->damage;
                    delete projectile;
                    it = projectilesPlayer.erase(it);
                    hit = true;
                    break;
                }
            }
            if (!hit) {
                ++it;
            }
        }
    }
}

void SceneMain::renderPlayerProjectiles() {
    for (auto projectile : projectilesPlayer) {
        SDL_Rect projectileRect = {static_cast<int>(projectile->position.x),
                                   static_cast<int>(projectile->position.y),
                                   projectile->width, projectile->height};
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL,
                       &projectileRect);
    }
}

void SceneMain::spawEnemy() {
    if (dis(gen) > 1 / 60.0f) {
        return;
    }
    Enemy *enemy = new Enemy(enemyTemplate);
    enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
    enemy->position.y = -enemy->height;
    enemies.push_back(enemy);
}

void SceneMain::updateEnemies(float deltaTime) {
    auto currentTime = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end();) {
        auto enemy = *it;
        enemy->position.y += enemy->speed * deltaTime;
        if (enemy->position.y > game.getWindowHeight()) {
            delete enemy;
            it = enemies.erase(it);
        } else {
            if (currentTime - enemy->lastShootTime > enemy->coolDown &&
                isDead == false) {
                shootEnemy(enemy);
                enemy->lastShootTime = currentTime;
            }
            if (enemy->currentHealth <= 0) {
                enemyExplode(enemy);
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void SceneMain::renderEnemies() {
    for (auto enemy : enemies) {
        SDL_Rect enemyRect = {static_cast<int>(enemy->position.x),
                              static_cast<int>(enemy->position.y), enemy->width,
                              enemy->height};
        SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemyRect);
    }
}

void SceneMain::shootEnemy(Enemy *enemy) {
    auto projectile = new ProjectileEnemy(projectileEnemyTemplate);
    projectile->position.x =
        enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y =
        enemy->position.y + enemy->height / 2 - projectile->height / 2;
    projectile->direction = getDirection(enemy);
    projectilesEnemy.push_back(projectile);
}

void SceneMain::enemyExplode(Enemy *enemy) { delete enemy; }

SDL_FPoint SceneMain::getDirection(Enemy *enemy) {
    // 计算从敌机中心到玩家中心的向量
    auto x = (player.position.x + player.width / 2) -
             (enemy->position.x + enemy->width / 2);
    auto y = (player.position.y + player.height / 2) -
             (enemy->position.y + enemy->height / 2);
    // 计算向量长度
    auto length = sqrt(x * x + y * y);
    // 归一化向量
    x /= length;
    y /= length;
    return SDL_FPoint{x, y};
}

void SceneMain::updateEnemyProjectiles(float deltaTime) {
    auto margin = 32;
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();) {
        auto projectile = *it;
        projectile->position.x +=
            projectile->speed * projectile->direction.x * deltaTime;
        projectile->position.y +=
            projectile->speed * projectile->direction.y * deltaTime;
        if (projectile->position.y > game.getWindowHeight() + margin ||
            projectile->position.y < -margin ||
            projectile->position.x < -margin ||
            projectile->position.x > game.getWindowWidth() + margin) {
            delete projectile;
            it = projectilesEnemy.erase(it);
        } else {
            SDL_Rect projectileRect = {static_cast<int>(projectile->position.x),
                                       static_cast<int>(projectile->position.y),
                                       projectile->width, projectile->height};
            SDL_Rect playerRect = {static_cast<int>(player.position.x),
                                   static_cast<int>(player.position.y),
                                   player.width, player.height};
            if (SDL_HasIntersection(&projectileRect, &playerRect) && !isDead) {
                player.currentHealth -= projectile->damage;
                delete projectile;
                it = projectilesEnemy.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void SceneMain::renderEnemyProjectiles() {
    for (auto projectile : projectilesEnemy) {
        SDL_Rect projectileRect = {static_cast<int>(projectile->position.x),
                                   static_cast<int>(projectile->position.y),
                                   projectile->width, projectile->height};

        // 计算子弹旋转角度（从弧度转换为角度，并调整为正确的方向）
        float angle = atan2(projectile->direction.y, projectile->direction.x) *
                          180 / M_PI -
                      90;

        // 使用带旋转参数的渲染函数
        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, NULL,
                         &projectileRect, angle, NULL, SDL_FLIP_NONE);
    }
}

void SceneMain::updatePlayer(float deltaTime) {
    if (isDead) {
        return;
    }
    if (player.currentHealth <= 0) {
        // 玩家死亡处理
        isDead = true;
    }
    for (auto enemy : enemies) {
        SDL_Rect enemyRect = {static_cast<int>(enemy->position.x),
                              static_cast<int>(enemy->position.y), enemy->width,
                              enemy->height};
        SDL_Rect playerRect = {static_cast<int>(player.position.x),
                               static_cast<int>(player.position.y),
                               player.width, player.height};
        if (SDL_HasIntersection(&playerRect, &enemyRect)) {
            player.currentHealth -= 1;
            enemy->currentHealth = 0;
        }
    }
}