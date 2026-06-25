#pragma once
#include "DxLib.h"
#include <vector>

struct Star {
    VECTOR       pos;
    float        size;
    unsigned int color;
    float        phase;  // twinkle offset 0..2π
    int          layer;  // 0=far small, 1=near large
};

// biomeIdx: 0=Deep Space (Block1), 1=Nebula (Block2),
//           2=Gas Giant (Block3), 3=Void (Block4), 4=Ice Planet (Block5)

// Сферический скайбокс с текстурой (equirectangular PNG/JPG)
// graphHandle == -1 → fallback на процедурный DrawSpaceSkybox
void DrawSkyboxSphere(VECTOR playerPos, int graphHandle, float radius = 480.0f);

// Процедурный скайбокс (цветные боксы) — используется как fallback
void DrawSpaceSkybox(VECTOR playerPos, int biomeIdx = 0);
void DrawBiomePlanet(VECTOR playerPos, int biomeIdx);

std::vector<Star> GenerateStars(int count, float radius, int biomeIdx = 0);
// playerPos используется как центр звёздного поля (звёзды следуют за игроком)
void DrawStars(const std::vector<Star>& stars, VECTOR playerPos);

void ApplyBiomeFog(int biomeIdx);
void DisableFog();
