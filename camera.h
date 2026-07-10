#pragma once
#include "DxLib.h"
#include "Constants.h"

class Camera {
public:
    Camera();

    // Обновляет углы по движению мыши (позицию НЕ трогает) — вызывать ДО player.Update(),
    // чтобы игрок двигался относительно направления взгляда уже этого кадра
    void UpdateAngles(float sensitivity);

    // Применяет позицию/цель камеры к текущим углам — вызывать ПОСЛЕ player.Update(),
    // чтобы камера следовала за позицией игрока уже этого кадра, а не прошлого
    // (иначе модель на быстром падении визуально "отстаёт" от камеры на кадр)
    void Apply(VECTOR targetPos) const;

    VECTOR GetPosition(VECTOR targetPos) const;
    float  GetAngleY() const { return angleY; }
    float  GetAngleX() const { return angleX; }

private:
    float angleX;
    float angleY;
    float distance;

    static constexpr float ANGLE_X_MAX = 1.5f;
};
