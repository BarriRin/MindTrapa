#include "Level.h"

Level::Level(int id) : levelId(id) {
    LoadLevelData(id);
}

Level::~Level() {
    blocks.clear();
}

void Level::LoadLevelData(int id) {
    blocks.clear();

    switch (id) {
    case 1: // Уровень 1 - простой туториал
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(10, 1, 10), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 2: // Уровень 2 - прыжки + первые шипы!
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 1, -2), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(12, 3, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 2, 0), VGet(1, 1, 1), BlockType::SPIKES));
        blocks.push_back(Block(VGet(15, 4, 2), VGet(1, 1, 1), BlockType::SPIKES));
        blocks.push_back(Block(VGet(20, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 3: // Уровень 3 - невидимые стены
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 0, -2), VGet(1, 4, 6), BlockType::INVISIBLE_WALL));
        blocks.push_back(Block(VGet(3, 1, 5), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 7), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 4: // Уровень 4 - телепорты
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(2, 1, 2), BlockType::TELEPORT, 0.0f, true, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(20, 3, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(21, 4, 1), VGet(2, 1, 2), BlockType::TELEPORT, 0.0f, true, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(30, 5, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(32, 6, 1), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 5: // Уровень 5 - ложные телепорты
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, -3), VGet(2, 1, 2), BlockType::TELEPORT, 0.0f, true, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(2, 1, 2), BlockType::TELEPORT, 0.0f, true, VGet(0, 0, 0), 2));
        blocks.push_back(Block(VGet(8, 1, 3), VGet(2, 1, 2), BlockType::TELEPORT, 0.0f, true, VGet(0, 0, 0), 3));
        blocks.push_back(Block(VGet(20, 1, -3), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 2, -3), VGet(1, 1, 1), BlockType::SPIKES));
        blocks.push_back(Block(VGet(20, 1, 3), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(25, 1, 3), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 6: // Уровень 6 - кнопки и двери
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(2, 1, 2), BlockType::BUTTON, 0.0f, false, VGet(0, 0, 0), 10));
        blocks.push_back(Block(VGet(15, 1, 0), VGet(4, 1, 4), BlockType::DISAPPEARING, 0.0f, false, VGet(0, 0, 0), 10));
        blocks.push_back(Block(VGet(22, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 7: // Уровень 7 - движущиеся платформы
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 1, 0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(10, 2, 0), VGet(4, 1, 4), BlockType::MOVING, 0.0f, true, VGet(20, 2, 0), 0));
        blocks.push_back(Block(VGet(25, 3, 0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(30, 4, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 8: // Уровень 8 - исчезающие платформы
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 2, 0), VGet(3, 1, 3), BlockType::DISAPPEARING, 0.0f, true, VGet(0, 0, 0), 0));
        blocks.push_back(Block(VGet(10, 3, 0), VGet(3, 1, 3), BlockType::DISAPPEARING, 0.0f, true, VGet(0, 0, 0), 0));
        blocks.push_back(Block(VGet(15, 4, 0), VGet(2, 1, 2), BlockType::DISAPPEARING, 0.0f, true, VGet(0, 0, 0), 0));
        blocks.push_back(Block(VGet(20, 5, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(25, 5, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 9: // Уровень 9 - выдвигающиеся шипы
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Первая секция - узкий коридор
        blocks.push_back(Block(VGet(5, 0, -1), VGet(12, 1, 3), BlockType::PLATFORM));
        // Шипы на всю ширину платформы (3 единицы)
        blocks.push_back(Block(VGet(7, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 0.0f, true, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(10, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 2.0f, false, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(13, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 0.0f, true, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(16, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 2.0f, false, VGet(0, 0, 0), 1));

        // Вторая секция - немного шире
        blocks.push_back(Block(VGet(20, 2, -2), VGet(8, 1, 5), BlockType::PLATFORM));
        // Шипы на всю ширину (5 единиц)
        blocks.push_back(Block(VGet(21, 3, -2), VGet(1, 1, 5), BlockType::SPIKES, 0.0f, true, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(23, 3, -2), VGet(1, 1, 5), BlockType::SPIKES, 1.0f, false, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(25, 3, -2), VGet(1, 1, 5), BlockType::SPIKES, 0.0f, true, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(27, 3, -2), VGet(1, 1, 5), BlockType::SPIKES, 1.0f, false, VGet(0, 0, 0), 1));

        // Третья секция - финальный рывок
        blocks.push_back(Block(VGet(31, 3, -1), VGet(10, 1, 3), BlockType::PLATFORM));
        // Много шипов подряд, нужно выбрать момент
        blocks.push_back(Block(VGet(32, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 0.0f, true, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(34, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 0.5f, true, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(36, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 1.0f, false, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(38, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 1.5f, false, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(40, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 2.0f, false, VGet(0, 0, 0), 1));

        // Финиш
        blocks.push_back(Block(VGet(44, 4, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(46, 5, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 10: // Уровень 10 - ФИНАЛ БЛОКА 1! Все механики обязательны
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === СЕКЦИЯ 1: Выдвигающиеся шипы (обязательно пройти) ===
        blocks.push_back(Block(VGet(5, 0, -1), VGet(10, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(7, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 0.0f, true, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(10, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 0.0f, true, VGet(0, 0, 0), 0)); // Статичные!
        blocks.push_back(Block(VGet(13, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 2.0f, false, VGet(0, 0, 0), 1));

        // === СЕКЦИЯ 2: Движущаяся платформа - ОБЯЗАТЕЛЬНА (большая пропасть) ===
        // Слишком далеко чтобы прыгнуть напрямую!
        blocks.push_back(Block(VGet(18, 2, -1), VGet(3, 1, 3), BlockType::PLATFORM)); // Маленькая площадка
        // Движущаяся платформа - единственный путь
        blocks.push_back(Block(VGet(22, 3, -1), VGet(4, 1, 3), BlockType::MOVING, 0.0f, true, VGet(35, 3, -1), 0));
        // Целевая платформа далеко
        blocks.push_back(Block(VGet(38, 4, -2), VGet(5, 1, 5), BlockType::PLATFORM));

        // === СЕКЦИЯ 3: Телепорт - ОБЯЗАТЕЛЕН (невидимые стены блокируют обход) ===
        blocks.push_back(Block(VGet(30, 10, -2), VGet(4, 1, 5), BlockType::PLATFORM)); // Высоко!
        blocks.push_back(Block(VGet(31, 11, 0), VGet(2, 1, 2), BlockType::TELEPORT, 0.0f, true, VGet(0, 0, 0), 10));
        // Невидимые стены со всех сторон - нельзя обойти!
        blocks.push_back(Block(VGet(34, 10, -2), VGet(1, 4, 5), BlockType::INVISIBLE_WALL)); // Справа
        blocks.push_back(Block(VGet(29, 10, -2), VGet(1, 4, 5), BlockType::INVISIBLE_WALL)); // Слева

        // Куда телепортирует (далеко и высоко)
        blocks.push_back(Block(VGet(50, 6, -3), VGet(6, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(52, 7, 0), VGet(2, 1, 2), BlockType::TELEPORT, 0.0f, true, VGet(0, 0, 0), 10));

        // === СЕКЦИЯ 4: Кнопка + исчезающие платформы - ВСЁ ОБЯЗАТЕЛЬНО ===
        blocks.push_back(Block(VGet(60, 7, -2), VGet(4, 1, 5), BlockType::PLATFORM));
        // Кнопка
        blocks.push_back(Block(VGet(61, 8, 0), VGet(2, 1, 2), BlockType::BUTTON, 0.0f, false, VGet(0, 0, 0), 20));

        // Исчезающие платформы к двери (расстояния большие!)
        blocks.push_back(Block(VGet(67, 8, 0), VGet(2, 1, 2), BlockType::DISAPPEARING, 0.0f, true, VGet(0, 0, 0), 0));
        blocks.push_back(Block(VGet(72, 9, 0), VGet(2, 1, 2), BlockType::DISAPPEARING, 0.0f, true, VGet(0, 0, 0), 0));
        blocks.push_back(Block(VGet(77, 10, 0), VGet(2, 1, 2), BlockType::DISAPPEARING, 0.0f, true, VGet(0, 0, 0), 0));

        // Дверь (открывается только кнопкой) - блокирует путь!
        blocks.push_back(Block(VGet(81, 10, -1), VGet(1, 4, 3), BlockType::DISAPPEARING, 0.0f, false, VGet(0, 0, 0), 20));
        // Невидимая стена сверху чтобы не перепрыгнуть дверь
        blocks.push_back(Block(VGet(81, 14, -1), VGet(1, 5, 3), BlockType::INVISIBLE_WALL));

        // === ФИНАЛ: Последний рывок через выдвигающиеся шипы ===
        blocks.push_back(Block(VGet(84, 11, -1), VGet(8, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(85, 12, -1), VGet(1, 1, 3), BlockType::SPIKES, 0.0f, true, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(87, 12, -1), VGet(1, 1, 3), BlockType::SPIKES, 1.0f, false, VGet(0, 0, 0), 1));
        blocks.push_back(Block(VGet(89, 12, -1), VGet(1, 1, 3), BlockType::SPIKES, 2.0f, false, VGet(0, 0, 0), 1));

        // Финальная платформа и триггер
        blocks.push_back(Block(VGet(94, 12, -3), VGet(6, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(97, 13, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    default:
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-3, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;
    }
}

void Level::Draw() const {
    for (const auto& block : blocks) {
        switch (block.type) {
        case BlockType::PLATFORM:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(200, 200, 200), GetColor(150, 150, 150), TRUE);
            break;

        case BlockType::TRIGGER:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(100, 200, 255), GetColor(50, 150, 200), TRUE);
            break;

        case BlockType::INVISIBLE_WALL:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
            break;

        case BlockType::FAKE_PLATFORM:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(180, 180, 180), GetColor(130, 130, 130), TRUE);
            break;

        case BlockType::SPIKES:
            if (block.linkId == 1) {
                if (block.isActive) {
                    DrawCube3D(block.pos, VAdd(block.pos, block.size),
                        GetColor(255, 50, 50), GetColor(200, 0, 0), TRUE);
                }
                else {
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
                    DrawCube3D(block.pos, VAdd(block.pos, block.size),
                        GetColor(150, 150, 150), GetColor(100, 100, 100), TRUE);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                }
            }
            else {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 50, 50), GetColor(200, 0, 0), TRUE);
            }
            break;

        case BlockType::DISAPPEARING:
            if (block.isActive) {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 255, 100), GetColor(200, 200, 50), TRUE);
            }
            break;

        case BlockType::MOVING:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
            break;

        case BlockType::BUTTON:
            if (block.isActive) {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
            }
            else {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 255, 100), GetColor(200, 200, 50), TRUE);
            }
            break;

        case BlockType::TELEPORT:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 100, 255), GetColor(200, 50, 200), TRUE);
            break;
        }
    }
}

bool Level::CheckCollision(VECTOR playerPos, VECTOR playerSize, VECTOR& newPos, VECTOR& velocity, bool& onGround, VECTOR& platformVelocity) {
    onGround = false;
    platformVelocity = VGet(0, 0, 0);

    for (auto& block : blocks) {
        if (block.type == BlockType::PLATFORM ||
            block.type == BlockType::INVISIBLE_WALL ||
            block.type == BlockType::MOVING ||
            block.type == BlockType::BUTTON ||
            (block.type == BlockType::DISAPPEARING && block.isActive)) {

            if (newPos.x < block.pos.x + block.size.x && newPos.x + playerSize.x > block.pos.x &&
                newPos.y < block.pos.y + block.size.y && newPos.y + playerSize.y > block.pos.y &&
                newPos.z < block.pos.z + block.size.z && newPos.z + playerSize.z > block.pos.z) {

                float overlapX = 0, overlapY = 0, overlapZ = 0;

                if (newPos.x < block.pos.x) {
                    overlapX = (newPos.x + playerSize.x) - block.pos.x;
                }
                else {
                    overlapX = (block.pos.x + block.size.x) - newPos.x;
                }

                if (newPos.y < block.pos.y) {
                    overlapY = (newPos.y + playerSize.y) - block.pos.y;
                }
                else {
                    overlapY = (block.pos.y + block.size.y) - newPos.y;
                }

                if (newPos.z < block.pos.z) {
                    overlapZ = (newPos.z + playerSize.z) - block.pos.z;
                }
                else {
                    overlapZ = (block.pos.z + block.size.z) - newPos.z;
                }

                if (overlapY <= overlapX && overlapY <= overlapZ) {
                    if (velocity.y <= 0 && playerPos.y > block.pos.y + block.size.y - 1.5f) {
                        newPos.y = block.pos.y + block.size.y;
                        velocity.y = 0;
                        onGround = true;

                        if (block.type == BlockType::MOVING) {
                            platformVelocity = VGet(
                                block.pos.x - block.prevPos.x,
                                block.pos.y - block.prevPos.y,
                                block.pos.z - block.prevPos.z
                            );
                        }

                        if (block.type == BlockType::DISAPPEARING && block.linkId == 0) {
                            if (block.timer == 0.0f) {
                                block.timer = 1.5f;
                            }
                        }
                    }
                    else if (velocity.y > 0) {
                        newPos.y = block.pos.y - playerSize.y;
                        velocity.y = 0;
                    }
                }
                else if (overlapX <= overlapZ) {
                    if (newPos.x < block.pos.x) {
                        newPos.x = block.pos.x - playerSize.x;
                    }
                    else {
                        newPos.x = block.pos.x + block.size.x;
                    }
                }
                else {
                    if (newPos.z < block.pos.z) {
                        newPos.z = block.pos.z - playerSize.z;
                    }
                    else {
                        newPos.z = block.pos.z + block.size.z;
                    }
                }

                return true;
            }
        }
    }
    return false;
}

bool Level::CheckWinTrigger(VECTOR playerPos, VECTOR playerSize) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::TRIGGER) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return true;
            }
        }
    }
    return false;
}

bool Level::CheckDeadlyTrigger(VECTOR playerPos, VECTOR playerSize) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::SPIKES) {
            if (block.linkId == 1 && !block.isActive) {
                continue;
            }

            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return true;
            }
        }
    }
    return false;
}

void Level::Update(float deltaTime) {
    for (auto& block : blocks) {
        if (block.type == BlockType::MOVING) {
            block.prevPos = block.pos;

            block.timer += deltaTime * 0.3f;

            if (block.timer > 1.0f) {
                block.timer = 1.0f;
                block.isActive = false;
            }
            if (block.timer < 0.0f) {
                block.timer = 0.0f;
                block.isActive = true;
            }

            if (!block.isActive) {
                block.timer -= deltaTime * 0.6f;
            }

            float t = block.timer;
            block.pos.x = block.originalPos.x + (block.moveTarget.x - block.originalPos.x) * t;
            block.pos.y = block.originalPos.y + (block.moveTarget.y - block.originalPos.y) * t;
            block.pos.z = block.originalPos.z + (block.moveTarget.z - block.originalPos.z) * t;
        }

        if (block.type == BlockType::DISAPPEARING) {
            if (block.linkId == 0 && block.timer > 0.0f) {
                block.timer -= deltaTime;

                if (block.timer <= 0.0f) {
                    block.isActive = false;
                    block.timer = -3.0f;
                }
            }

            if (block.linkId == 0 && block.timer < 0.0f) {
                block.timer += deltaTime;

                if (block.timer >= 0.0f) {
                    block.isActive = true;
                    block.timer = 0.0f;
                }
            }
        }

        if (block.type == BlockType::SPIKES && block.linkId == 1) {
            block.timer += deltaTime;

            float cycleTime = 4.0f;
            float phase = fmod(block.timer, cycleTime);

            if (phase < 2.0f) {
                block.isActive = true;
            }
            else {
                block.isActive = false;
            }
        }
    }
}

bool Level::CheckTeleportTrigger(VECTOR playerPos, VECTOR playerSize, VECTOR& teleportTarget) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::TELEPORT) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {

                for (const auto& otherBlock : blocks) {
                    if (otherBlock.type == BlockType::TELEPORT &&
                        otherBlock.linkId == block.linkId &&
                        &otherBlock != &block) {

                        teleportTarget = VGet(otherBlock.pos.x + otherBlock.size.x / 2,
                            otherBlock.pos.y + otherBlock.size.y + 1,
                            otherBlock.pos.z + otherBlock.size.z / 2);
                        return true;
                    }
                }

                if (block.linkId == 1) {
                    teleportTarget = VGet(25, 7, 0);
                    return true;
                }
                else if (block.linkId == 2) {
                    teleportTarget = VGet(5, 15, 0);
                    return true;
                }
                else if (block.linkId == 3) {
                    teleportTarget = VGet(20, 3, 3);
                    return true;
                }
            }
        }
    }
    return false;
}

void Level::ActivateButton(VECTOR playerPos, VECTOR playerSize, bool keyPressed) {
    for (auto& block : blocks) {
        if (block.type == BlockType::BUTTON) {
            float distance = sqrtf(
                (playerPos.x - (block.pos.x + block.size.x / 2)) * (playerPos.x - (block.pos.x + block.size.x / 2)) +
                (playerPos.z - (block.pos.z + block.size.z / 2)) * (playerPos.z - (block.pos.z + block.size.z / 2))
            );

            if (distance < 3.0f && keyPressed) {
                block.isActive = !block.isActive;

                for (auto& otherBlock : blocks) {
                    if (otherBlock.linkId == block.linkId && otherBlock.type == BlockType::DISAPPEARING) {
                        otherBlock.isActive = block.isActive;
                    }
                }
            }
        }
    }
}