#pragma once
#include "DxLib.h"

class Platform {
public:
    // Переименовали, чтобы никакие макросы min/max не лезли
    VECTOR minV, maxV;

    Platform(const VECTOR& min_, const VECTOR& max_);
    void    draw(int r, int g, int b) const;
    bool    checkCollision(const VECTOR& objMin, const VECTOR& objMax) const;
};
