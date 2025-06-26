#include "player.h"
#include <cmath>

Player::Player()
    : position(VGet(0.0f, 1.0f, 0.0f))
    , velocity(VGet(0.0f, 0.0f, 0.0f))
    , speed(0.4f)  // Чуть увеличим скорость
    , onGround(false)
{
}

void Player::update(const Camera& cam) {
    // 1) Собираем WASD
    float moveForward = 0, moveRight = 0;
    if (CheckHitKey(KEY_INPUT_W)) moveForward += 1;   // Вперед
    if (CheckHitKey(KEY_INPUT_S)) moveForward -= 1;   // Назад
    if (CheckHitKey(KEY_INPUT_D)) moveRight += 1;     // Вправо
    if (CheckHitKey(KEY_INPUT_A)) moveRight -= 1;     // Влево

    // 2) Получаем направления от камеры
    VECTOR forward = cam.getForwardVector();
    VECTOR right = cam.getRightVector();

    // 3) Вычисляем направление движения
    VECTOR moveDirection = VGet(0, 0, 0);

    // Добавляем движение вперед/назад
    moveDirection.x += forward.x * moveForward;
    moveDirection.z += forward.z * moveForward;

    // Добавляем движение влево/вправо
    moveDirection.x += right.x * moveRight;
    moveDirection.z += right.z * moveRight;

    // 4) Нормализуем вектор движения (чтобы диагональное движение не было быстрее)
    float length = sqrtf(moveDirection.x * moveDirection.x + moveDirection.z * moveDirection.z);
    if (length > 0.001f) {
        moveDirection.x = (moveDirection.x / length) * speed;
        moveDirection.z = (moveDirection.z / length) * speed;
    }

    // 5) Применяем скорость по X и Z
    velocity.x = moveDirection.x;
    velocity.z = moveDirection.z;

    // 6) Прыжок и гравитация
    if (onGround && CheckHitKey(KEY_INPUT_SPACE)) {
        velocity.y = 0.8f;  // Чуть увеличим силу прыжка
        onGround = false;
    }
    velocity.y -= 0.03f;  // Гравитация

    // 7) Двигаем игрока
    position = VAdd(position, velocity);
}

void Player::resolveCollision(const Platform& floor) {
    VECTOR pMin = getAABBMin();
    VECTOR pMax = getAABBMax();

    // 1) Столкновение по Y (пол)
    if (floor.checkCollision(pMin, pMax) && velocity.y <= 0.0f) {
        position.y = floor.maxV.y + 2.01f;  // Высота игрока + небольшой отступ
        velocity.y = 0.0f;
        onGround = true;
    }
    else {
        onGround = false;
    }

    // 2) Зажим по X/Z — не выезжать за край пола
    float xmin = floor.minV.x + 1.0f;
    float xmax = floor.maxV.x - 1.0f;
    float zmin = floor.minV.z + 1.0f;
    float zmax = floor.maxV.z - 1.0f;

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