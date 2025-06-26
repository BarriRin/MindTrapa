#include "platform.h"

Platform::Platform(const VECTOR& min_, const VECTOR& max_)
    : minV(min_), maxV(max_)
{
}

void Platform::draw(int r, int g, int b) const {
    DrawCube3D(
        minV,
        maxV,
        GetColor(r, g, b),
        GetColor(r / 2, g / 2, b / 2),
        TRUE
    );
}

bool Platform::checkCollision(const VECTOR& objMin, const VECTOR& objMax) const {
    // AABB collision detection
    return !(objMax.x < minV.x || objMin.x > maxV.x ||
        objMax.y < minV.y || objMin.y > maxV.y ||
        objMax.z < minV.z || objMin.z > maxV.z);
}