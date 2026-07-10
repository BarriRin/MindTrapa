#include "Camera.h"
#include <cmath>

Camera::Camera()
    : angleX(0.0f), angleY(0.0f), distance(10.0f)
{}

VECTOR Camera::GetPosition(VECTOR targetPos) const {
    return VGet(
        targetPos.x + distance * sinf(angleY) * cosf(angleX),
        targetPos.y + distance * sinf(angleX) + 2.0f,
        targetPos.z + distance * cosf(angleY) * cosf(angleX)
    );
}

void Camera::Apply(VECTOR targetPos) const {
    VECTOR camPos = GetPosition(targetPos);
    SetCameraPositionAndTarget_UpVecY(camPos, VAdd(targetPos, VGet(0, 1, 0)));
}

void Camera::UpdateAngles(float sensitivity) {
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);

    int centerX = SCREEN_W / 2;
    int centerY = SCREEN_H / 2;

    angleY += (mouseX - centerX) * 0.002f * sensitivity;
    angleX += (mouseY - centerY) * 0.002f * sensitivity;

    if (angleX >  ANGLE_X_MAX) angleX =  ANGLE_X_MAX;
    if (angleX < -ANGLE_X_MAX) angleX = -ANGLE_X_MAX;

    SetMousePoint(centerX, centerY);
}
