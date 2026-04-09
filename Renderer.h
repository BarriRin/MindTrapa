#pragma once
#include "DxLib.h"
#include <vector>

struct Star {
    VECTOR pos;
    float size;
    unsigned int color;
};

void DrawSpaceSkybox(VECTOR playerPos);
std::vector<Star> GenerateStars(int count, float radius);
void DrawStars(const std::vector<Star>& stars, VECTOR cameraPos);
