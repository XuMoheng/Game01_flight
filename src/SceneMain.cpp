// SceneMain.cpp
#include "SceneMain.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>

SceneMain::SceneMain() : game(Game::getInstance()) {}

SceneMain::~SceneMain() {}

void SceneMain::update() { keyboardControl(); }

void SceneMain::render() {
    SDL_Rect playerRect = {static_cast<int>(player.position.x),
                           static_cast<int>(player.position.y), player.width,
                           player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
}

void SceneMain::handleEvent(SDL_Event *event) {}

void SceneMain::init() {
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
}

void SceneMain::clean() {
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
    }
}

void SceneMain::keyboardControl() {
    auto keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_W]) {
        player.position.y -= 1;
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        player.position.y += 1;
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        player.position.x -= 1;
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        player.position.x += 1;
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
}