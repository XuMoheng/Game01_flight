// SceneMain.cpp
#include "SceneMain.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_scancode.h>

SceneMain::SceneMain() : game(Game::getInstance()) {}

SceneMain::~SceneMain() {}

void SceneMain::init() {
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
}

void SceneMain::handleEvent([[maybe_unused]] SDL_Event *event) {}

void SceneMain::update(float deltaTime) {
    keyboardControl(deltaTime);
    updatePlayerProjectile(deltaTime);
}

void SceneMain::render() {
    renderPlayerProjectiles();
    SDL_Rect playerRect = {static_cast<int>(player.position.x),
                           static_cast<int>(player.position.y), player.width,
                           player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
}

void SceneMain::clean() {
    // 清理子弹
    for (auto &projectile : projectilesPlayer) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilesPlayer.clear();

    // 清理纹理
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
    }
    if (projectilePlayerTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
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
        if (currentTime - player.lastShootTime > player.coolDown) {
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

void SceneMain::updatePlayerProjectile(float deltaTime) {
    int margin = 32; // 子弹与窗口边界的距离
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();) {
        auto projectile = *it;
        projectile->position.y -= projectile->speed * deltaTime;
        if (projectile->position.y + margin < 0) {
            delete projectile;
            it = projectilesPlayer.erase(it);
        } else {
            ++it;
        }
    }
}

void SceneMain::renderPlayerProjectiles()
{
    for (auto projectile : projectilesPlayer){
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
    }
}
