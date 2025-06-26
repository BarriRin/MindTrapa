// camera.cpp
#include "camera.h"
#define NOMINMAX
#include <DxLib.h>
#include <cmath>
#include <cfloat>

static float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

Camera::Camera()
    : target(nullptr)
    , yaw(0.0f)
    , pitch(10.0f)
{
    SetMouseDispFlag(FALSE);
    int w, h;
    GetDrawScreenSize(&w, &h);
    SetMousePoint(w / 2, h / 2);

    // Настраиваем параметры камеры DxLib
    SetCameraNearFar(1.0f, 1000.0f);  // Ближняя и дальняя плоскости
    SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);  // Угол обзора 60 градусов
}

void Camera::setTarget(const VECTOR* t) {
    target = t;
}

void Camera::update() {
    int screenW, screenH;
    GetDrawScreenSize(&screenW, &screenH);

    int mx, my;
    GetMousePoint(&mx, &my);

    int dx = mx - screenW / 2;
    int dy = my - screenH / 2;

    SetMousePoint(screenW / 2, screenH / 2);

    // Более плавная чувствительность мыши
    yaw += dx * 0.15f;
    pitch -= dy * 0.15f;

    // Ограничиваем pitch (вертикальный угол)
    pitch = clampf(pitch, -85.0f, 85.0f);

    if (target) {
        float ry = yaw * DX_PI_F / 180.0f;
        float rp = pitch * DX_PI_F / 180.0f;

        // Камера должна быть ближе и ниже, чтобы видеть игрока
        float distance = 8.0f;   // Ближе
        float height = 3.0f;     // Ниже

        // Позиция камеры за игроком
        float cx = target->x + std::sinf(ry) * distance;
        float cz = target->z + std::cosf(ry) * distance;
        float cy = target->y + height + std::sinf(rp) * 3.0f;

        // Смотрим прямо на игрока
        VECTOR camPos = VGet(cx, cy, cz);
        VECTOR lookAt = VGet(target->x, target->y, target->z);
        VECTOR upVec = VGet(0.0f, 1.0f, 0.0f);

        SetCameraPositionAndTargetAndUpVec(camPos, lookAt, upVec);
    }
}

VECTOR Camera::getForwardVector() const {
    float ry = yaw * DX_PI_F / 180.0f;
    // Направление "вперед" = туда, куда смотрит камера горизонтально
    return VGet(-std::sinf(ry), 0.0f, -std::cosf(ry));
}

VECTOR Camera::getRightVector() const {
    float ry = yaw * DX_PI_F / 180.0f;
    // Направление "вправо"
    return VGet(-std::cosf(ry), 0.0f, std::sinf(ry));
}