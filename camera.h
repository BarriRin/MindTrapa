#pragma once
#define NOMINMAX
#include "DxLib.h"

class Camera {
public:
    Camera();
    void   setTarget(const VECTOR* t);
    void   update();
    float  getYaw() const { return yaw; }

private:
    const VECTOR* target;
    float         yaw;
    float         pitch;

    // для «ручного» подсчёта движения мыши
    int           prevMx, prevMy;
};
