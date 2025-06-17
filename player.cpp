#include "player.h"
#include <cmath>  // sinf, cosf

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
    float fx = cosf(yawRad), fz = sinf(yawRad);
    float rx = -sinf(yawRad), rz = cosf(yawRad);

    velocity.x = (fx * moveF + rx * moveR) * speed;
    velocity.z = (fz * moveF + rz * moveR) * speed;

    if (onGround && CheckHitKey(KEY_INPUT_SPACE)) {
        velocity.y = 0.7f;
        onGround = false;
    }
    // гравитация
    velocity.y -= 0.035f;

    position = VAdd(position, velocity);
}

void Player::resolveCollision(const Platform& floor) {
    VECTOR pMin = getAABBMin(), pMax = getAABBMax();
    if (floor.checkCollision(pMin, pMax) && velocity.y <= 0) {
        // «встать» на поверхность
        position.y = floor.maxV.y + 0.01f;
        velocity.y = 0;
        onGround = true;
    }
    else {
        onGround = false;
    }

    // зажимаем X внутрь платформы
    float minX = floor.minV.x + 1.0f;
    float maxX = floor.maxV.x - 1.0f;
    if (position.x < minX) position.x = minX;
    if (position.x > maxX) position.x = maxX;

    // зажимаем Z внутрь платформы
    float minZ = floor.minV.z + 1.0f;
    float maxZ = floor.maxV.z - 1.0f;
    if (position.z < minZ) position.z = minZ;
    if (position.z > maxZ) position.z = maxZ;
}

void Player::draw() const {
    DrawCube3D(getAABBMin(), getAABBMax(),
        GetColor(50, 255, 50),
        GetColor(0, 200, 0),
        TRUE);
}

VECTOR Player::getAABBMin() const {
    return VGet(position.x - 1, position.y - 2, position.z - 1);
}

VECTOR Player::getAABBMax() const {
    return VGet(position.x + 1, position.y, position.z + 1);
}
