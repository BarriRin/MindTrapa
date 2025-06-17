#pragma once
#include "DxLib.h"

class Platform {
public:
    VECTOR minPos, maxPos;
    Platform(const VECTOR& min_, const VECTOR& max_);
    void    draw(int r, int g, int b) const;
    bool    checkCollision(const VECTOR& objMin, const VECTOR& objMax) const;
};
