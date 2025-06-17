#include "platform.h"

Platform::Platform(const VECTOR& min_, const VECTOR& max_)
    : minV(min_), maxV(max_)
{
}

void Platform::draw(int r, int g, int b) const {
    DrawCube3D(minV, maxV,
        GetColor(r, g, b),
        GetColor(r, g, b),
        TRUE);
}

bool Platform::checkCollision(const VECTOR& objMin,
    const VECTOR& objMax) const {
    return (objMin.x <= maxV.x && objMax.x >= minV.x) &&
        (objMin.y <= maxV.y && objMax.y >= minV.y) &&
        (objMin.z <= maxV.z && objMax.z >= minV.z);
}
