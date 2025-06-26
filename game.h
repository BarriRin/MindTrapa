#pragma once
#define NOMINMAX
#include "camera.h"
#include "player.h"
#include "platform.h"

class Game {
public:
    Game();
    void update();
    void draw();

private:
    Camera   camera;
    Player   player;
    Platform floor;
    Platform triggerCube;
    bool     nextLevel;
};