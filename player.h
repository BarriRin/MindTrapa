#pragma once
#include "DxLib.h"
#include "camera.h"
#include "platform.h"

class Player {
public:
    VECTOR position;
    VECTOR velocity;
    float  speed;
    bool   onGround;

    Player();
    void   update(const Camera& cam);
    void   resolveCollision(const Platform& floor);
    void   draw() const;
    VECTOR getAABBMin() const;
    VECTOR getAABBMax() const;
};
