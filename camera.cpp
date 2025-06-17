#include "camera.h"
#include <DxLib.h>
#include <cmath>

Camera::Camera()
    : target(nullptr)
    , yaw(0.0f)
    , pitch(45.0f)
    , prevMx(0)
    , prevMy(0)
{
    // захватим стартовый курсор, чтобы не получить «скачок» на первом кадре
    GetMousePoint(&prevMx, &prevMy);
}

void Camera::setTarget(const VECTOR* t) {
    target = t;
}

void Camera::update() {
    int mx, my;
    GetMousePoint(&mx, &my);        // абсолютная позиция мыши
    int dx = mx - prevMx, dy = my - prevMy;
    prevMx = mx;  prevMy = my;

    constexpr float SENS_X = 0.3f;
    constexpr float SENS_Y = 0.3f;

    yaw += dx * SENS_X;
    pitch -= dy * SENS_Y;           // инверт Y

    if (pitch < 10.0f) pitch = 10.0f;
    if (pitch > 80.0f) pitch = 80.0f;

    if (target) {
        float rY = yaw * DX_PI_F / 180.0f;
        float rP = pitch * DX_PI_F / 180.0f;
        float dist = 30.0f;

        float cx = target->x + cosf(rY) * dist * cosf(rP);
        float cz = target->z + sinf(rY) * dist * cosf(rP);
        float cy = target->y + dist * sinf(rP);

        SetCameraPositionAndTarget_UpVecY(
            VGet(cx, cy, cz),
            *target
        );
    }
}
