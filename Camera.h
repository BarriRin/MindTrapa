#pragma once
#include "DxLib.h"
#include "Constants.h"

// Third-person orbit camera: angles come from mouse input, position is derived
// from the current angles and the player's position.
class Camera {
public:
    Camera();

    // Updates angles from mouse movement (does NOT touch position) — call BEFORE
    // player.Update(), so the player moves relative to this frame's look direction
    void UpdateAngles(float sensitivity);

    // Applies the camera's position/target from the current angles — call AFTER
    // player.Update(), so the camera follows this frame's player position rather
    // than last frame's (otherwise the model visibly "lags" the camera by a frame
    // during a fast fall)
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
