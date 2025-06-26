#pragma once
#include "DxLib.h"

class Camera {
public:
    Camera();
    void setTarget(const VECTOR* t);
    void update();

    float getYaw() const { return yaw; }
    VECTOR getForwardVector() const;  // Направление "вперед"
    VECTOR getRightVector() const;    // Направление "вправо"

private:
    const VECTOR* target;
    float yaw, pitch;
};