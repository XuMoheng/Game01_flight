// SceneMain.h
#pragma once
#include "Scene.h"

class SceneMain : public Scene {
  public:
    SceneMain();
    ~SceneMain();

    void update() override;
    void render() override;
    void handleEvent(SDL_Event *event) override;
    void init() override;
    void clean() override;

  private:
};
