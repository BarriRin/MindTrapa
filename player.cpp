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
    // 1) Собираем WASD
    float moveF = 0, moveR = 0;
    if (CheckHitKey(KEY_INPUT_W)) moveF += 1;
    if (CheckHitKey(KEY_INPUT_S)) moveF -= 1;
    if (CheckHitKey(KEY_INPUT_D)) moveR += 1;
    if (CheckHitKey(KEY_INPUT_A)) moveR -= 1;

    // 2) Направление вперёд/вправо относительно yaw
    float yawRad = cam.getYaw() * DX_PI_F / 180.0f;

    // вперёд = sin(yaw), −cos(yaw)  (чтобы W шло «вперёд» по -Z)
    float fx = sinf(yawRad);
    float fz = -cosf(yawRad);

    // вправо = cos(yaw), sin(yaw)
    float rx = cosf(yawRad);
    float rz = sinf(yawRad);

    // 3) Применяем скорость
    velocity.x = (fx * moveF + rx * moveR) * speed;
    velocity.z = (fz * moveF + rz * moveR) * speed;

    // 4) Прыжок и гравитация
    if (onGround && CheckHitKey(KEY_INPUT_SPACE)) {
        velocity.y = 0.7f;
        onGround = false;
    }
    velocity.y -= 0.03f;

    // 5) Двигаем
    position = VAdd(position, velocity);
}

void Player::resolveCollision(const Platform& floor) {
    VECTOR pMin = getAABBMin();
    VECTOR pMax = getAABBMax();

    // 1) Столкновение по Y
    if (floor.checkCollision(pMin, pMax) && velocity.y <= 0.0f) {
        position.y = floor.maxV.y + 0.01f;
        velocity.y = 0.0f;
        onGround = true;
    }
    else {
        onGround = false;
    }

    // 2) Зажим по X/Z — не выезжать за край
    float xmin = floor.minV.x + 1.0f;
    float xmax = floor.maxV.x - 1.0f;
    float zmin = floor.minV.z + 1.0f;
    float zmax = floor.maxV.z - 1.0f;

    // fminf/fmaxf — в <cmath>
    position.x = fmaxf(xmin, fminf(position.x, xmax));
    position.z = fmaxf(zmin, fminf(position.z, zmax));
}

void Player::draw() const {
    DrawCube3D(
        getAABBMin(),
        getAABBMax(),
        GetColor(50, 255, 50),
        GetColor(0, 200, 0),
        TRUE
    );
}

VECTOR Player::getAABBMin() const {
    return VGet(position.x - 1, position.y - 2, position.z - 1);
}

VECTOR Player::getAABBMax() const {
    return VGet(position.x + 1, position.y, position.z + 1);
}
