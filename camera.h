#pragma once
#include "DxLib.h"

class Camera {
public:
    Camera();
    void setTarget(const VECTOR* t);
    void update();
    float getYaw() const { return yaw; }
private:
    const VECTOR* target;
    float yaw, pitch;
};
