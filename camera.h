#pragma once
#include "DxLib.h"

class Camera {
public:
    Camera();

    // Обновляет углы по движению мыши и применяет SetCameraPositionAndTarget
    void Update(VECTOR targetPos, float sensitivity, float deltaTime);

    // Применяет позицию камеры без обработки ввода (для пауза/result экранов)
    void Apply(VECTOR targetPos) const;

    VECTOR GetPosition(VECTOR targetPos) const;
    float  GetAngleY() const { return angleY; }
    float  GetAngleX() const { return angleX; }

private:
    float angleX;
    float angleY;
    float distance;

    static constexpr int   SCREEN_W = 1920;
    static constexpr int   SCREEN_H = 1080;
    static constexpr float ANGLE_X_MAX = 1.5f;
};
