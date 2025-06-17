#include "camera.h"
#include <cmath>

Camera::Camera()
    : target(nullptr)
    , yaw(0.0f)
    , pitch(45.0f)
    , prevMx(0)
    , prevMy(0)
{
    // Инициализируем сразу текущей точкой мыши,
    // чтобы в первый кадр не получить огромный «шаг»
    GetMousePoint(&prevMx, &prevMy);
}

void Camera::setTarget(const VECTOR* t) {
    target = t;
}

void Camera::update() {
    // 1) Получаем текущую абсолютную позицию курсора
    int mx, my;
    GetMousePoint(&mx, &my);

    // 2) Вычисляем относительный сдвиг
    int dx = mx - prevMx;
    int dy = my - prevMy;

    // 3) Сохраняем для следующего кадра
    prevMx = mx;
    prevMy = my;

    // 4) Вращаем камеру
    yaw += dx * 0.2f;
    pitch -= dy * 0.2f;
    if (pitch < 5.0f) pitch = 5.0f;
    if (pitch > 80.0f) pitch = 80.0f;

    // 5) Если есть цель — рассчитываем позицию камеры
    if (target) {
        float ry = yaw * DX_PI_F / 180.0f;
        float rp = pitch * DX_PI_F / 180.0f;
        float dist = 30.0f;

        float cx = target->x + cosf(ry) * dist * cosf(rp);
        float cz = target->z + sinf(ry) * dist * cosf(rp);
        float cy = target->y + sinf(rp) * dist;

        SetCameraPositionAndTarget_UpVecY(
            VGet(cx, cy, cz),
            *target
        );
    }
}
