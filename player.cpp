#include "player.h"
#include <cmath>

Player::Player()
    : position(VGet(0.0f, 1.0f, 0.0f))
    , velocity(VGet(0.0f, 0.0f, 0.0f))
    , speed(0.3f)
    , onGround(false)
{
}

void Player::update(const Camera& cam) {
    float moveF = 0, moveR = 0;
    if (CheckHitKey(KEY_INPUT_W)) moveF += 1;
    if (CheckHitKey(KEY_INPUT_S)) moveF -= 1;
    if (CheckHitKey(KEY_INPUT_D)) moveR += 1;
    if (CheckHitKey(KEY_INPUT_A)) moveR -= 1;

    float yawRad = cam.getYaw() * DX_PI_F / 180.0f;
    float fx = sinf(yawRad), fz = cosf(yawRad);
    float rx = cosf(yawRad), rz = -sinf(yawRad);

    // движение по X/Z
    velocity.x = (fx * moveF + rx * moveR) * speed;
    velocity.z = (fz * moveF + rz * moveR) * speed;

    // прыжок
    if (onGround && CheckHitKey(KEY_INPUT_SPACE)) {
        velocity.y = 0.7f;
        onGround = false;
    }
    // гравитация
    velocity.y -= 0.03f;

    position = VAdd(position, velocity);
}

void Player::resolveCollision(const Platform& floor) {
    VECTOR pMin = getAABBMin(), pMax = getAABBMax();
    if (floor.checkCollision(pMin, pMax)) {
        // ставим на поверхность платформы
        position.y = floor.maxPos.y;
        velocity.y = 0;
        onGround = true;
    }
    else {
        onGround = false;
    }
}

void Player::draw() const {
    DrawCube3D(
        getAABBMin(), getAABBMax(),
        GetColor(50, 255, 50), GetColor(0, 200, 0), TRUE
    );
}

VECTOR Player::getAABBMin() const {
    return VGet(position.x - 1, position.y - 2, position.z - 1);
}

VECTOR Player::getAABBMax() const {
    return VGet(position.x + 1, position.y, position.z + 1);
}
