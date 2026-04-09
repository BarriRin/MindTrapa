#include "Renderer.h"
#include <random>

void DrawSpaceSkybox(VECTOR playerPos) {
    float skySize = 500.0f;
    VECTOR skyMin = VGet(playerPos.x - skySize, playerPos.y - skySize, playerPos.z - skySize);
    VECTOR skyMax = VGet(playerPos.x + skySize, playerPos.y + skySize, playerPos.z + skySize);

    SetUseZBuffer3D(FALSE);
    SetWriteZBuffer3D(FALSE);

    DrawCube3D(
        VGet(skyMin.x, playerPos.y, skyMin.z),
        VGet(skyMax.x, skyMax.y, skyMax.z),
        GetColor(5, 5, 20), GetColor(10, 10, 30), TRUE
    );

    DrawCube3D(
        VGet(skyMin.x, skyMin.y, skyMin.z),
        VGet(skyMax.x, playerPos.y, skyMax.z),
        GetColor(10, 10, 30), GetColor(15, 10, 25), TRUE
    );

    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
}

std::vector<Star> GenerateStars(int count, float radius) {
    std::vector<Star> stars;
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(-radius, radius);
    std::uniform_real_distribution<float> sizeDist(0.3f, 1.5f);

    for (int i = 0; i < count; i++) {
        Star star;
        star.pos = VGet(dist(gen), dist(gen), dist(gen));
        star.size = sizeDist(gen);

        int colorType = i % 3;
        if (colorType == 0) star.color = GetColor(255, 255, 255);
        else if (colorType == 1) star.color = GetColor(200, 220, 255);
        else star.color = GetColor(255, 240, 200);

        stars.push_back(star);
    }

    return stars;
}

void DrawStars(const std::vector<Star>& stars, VECTOR cameraPos) {
    SetUseZBuffer3D(FALSE);
    SetWriteZBuffer3D(FALSE);

    for (const auto& star : stars) {
        DrawCube3D(
            VGet(star.pos.x - star.size/2, star.pos.y - star.size/2, star.pos.z - star.size/2),
            VGet(star.pos.x + star.size/2, star.pos.y + star.size/2, star.pos.z + star.size/2),
            star.color, star.color, TRUE
        );
    }

    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
}
