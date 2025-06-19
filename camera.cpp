// camera.cpp
#include "camera.h"

// Отключаем макросы min/max из Win32
#define NOMINMAX
#include <DxLib.h>

// Подключаем только то, что нам реально нужно
#include <cmath>   // std::cosf, std::sinf
#include <cfloat>  // FLT_MAX

// Простая функция «зажатия» (clamp) без всяких шаблонов и макросов
static float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

Camera::Camera()
    : target(nullptr)
    , yaw(0.0f)
    , pitch(45.0f)
{
    // Скрываем курсор и центрируем его один раз
    SetMouseDispFlag(FALSE);
    int w, h;
    GetDrawScreenSize(&w, &h);
    SetMousePoint(w / 2, h / 2);
}

void Camera::setTarget(const VECTOR* t) {
    target = t;
}

void Camera::update() {
    // 1) Размер бэк-буфера
    int screenW, screenH;
    GetDrawScreenSize(&screenW, &screenH);

    // 2) Позиция мыши
    int mx, my;
    GetMousePoint(&mx, &my);

    // 3) Смещение от центра
    int dx = mx - screenW / 2;
    int dy = my - screenH / 2;

    // 4) Сбрасываем курсор в центр
    SetMousePoint(screenW / 2, screenH / 2);

    // 5) Меняем углы обзора
    yaw += dx * 0.2f;
    pitch -= dy * 0.2f;

    // 6) Ограничиваем pitch
    pitch = clampf(pitch, 5.0f, 80.0f);

    // 7) Если есть цель, вычисляем позицию камеры
    if (target) {
        float ry = yaw * DX_PI_F / 180.0f;
        float rp = pitch * DX_PI_F / 180.0f;
        float dist = 30.0f;

        float cx = target->x + std::cosf(ry) * dist * std::cosf(rp);
        float cz = target->z + std::sinf(ry) * dist * std::cosf(rp);
        float cy = target->y + std::sinf(rp) * dist;

        VECTOR camPos = VGet(cx, cy, cz);
        VECTOR upVec = VGet(0.0f, 1.0f, 0.0f);
        SetCameraPositionAndTargetAndUpVec(camPos, *target, upVec);
    }
}
