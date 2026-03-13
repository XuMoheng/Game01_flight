// Game.h
#pragma once

#include "Object.h"
#include "Scene.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <map>
#include <string>

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

    // 背景
    void backgroundUpdate(float deltaTime);
    void renderBackground();

    // 得分
    void setFinalScore(int score) { finalScore = score; }
    int getFinalScore() { return finalScore; }

    // 文字
    SDL_Point renderTextCentered(std::string text, float posY, bool isTitle);
    void renderTextPos(std::string text, int posX, int posY);
    void renderTextPos(std::string text, int posX, int posY, bool isLeft);

    // 排行榜
    void insertLeaderBoard(int score, std::string name);
    std::multimap<int, std::string, std::greater<int>> &getLeaderBoard() {
        return leaderBoard;
    }
    void saveData();
    void loadData();

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

    Background nearStars;
    Background farStars;

    int FPS = 120;
    Uint32 frameTime;
    float deltaTime;

    TTF_Font *titleFont;
    TTF_Font *textFont;

    int finalScore = 0;
    std::multimap<int, std::string, std::greater<int>> leaderBoard;
};
