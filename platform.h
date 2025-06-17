#pragma once
#include "DxLib.h"

class Platform {
public:
    VECTOR minV, maxV;                    // Å© ÑqÑçÑrÑäÑyÑu min/max
    Platform(const VECTOR& min_, const VECTOR& max_);
    void     draw(int r, int g, int b) const;
    bool     checkCollision(const VECTOR& objMin,
        const VECTOR& objMax) const;
};
