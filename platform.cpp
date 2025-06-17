#define NOMINMAX
#include "DxLib.h"
#include "platform.h"

Platform::Platform(const VECTOR& min_, const VECTOR& max_)
    : minPos(min_), maxPos(max_)
{
}

void Platform::draw(int r, int g, int b) const {
    DrawCube3D(minPos, maxPos, GetColor(r, g, b), GetColor(r, g, b), TRUE);
}

bool Platform::checkCollision(const VECTOR& objMin, const VECTOR& objMax) const {
    return (objMin.x <= maxPos.x && objMax.x >= minPos.x)
        && (objMin.y <= maxPos.y && objMax.y >= minPos.y)
        && (objMin.z <= maxPos.z && objMax.z >= minPos.z);
}
