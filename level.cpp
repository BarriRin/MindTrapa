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

        // Шипы торчат ИЗ платформ! 😈
        blocks.push_back(Block(VGet(8, 2, 0), VGet(1, 1, 1), BlockType::SPIKES));
        blocks.push_back(Block(VGet(15, 4, 2), VGet(1, 1, 1), BlockType::SPIKES));

        blocks.push_back(Block(VGet(20, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 3: // Уровень 3 - невидимые стены (первый троллинг!)
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Кажется что можно прыгнуть сюда...
        blocks.push_back(Block(VGet(8, 1, 0), VGet(4, 1, 4), BlockType::PLATFORM));

        // НО! Высокая невидимая стена блокирует прямой путь! 😈
        blocks.push_back(Block(VGet(5, 0, -2), VGet(1, 4, 6), BlockType::INVISIBLE_WALL));

        // Нужно идти в обход снизу или сверху
        blocks.push_back(Block(VGet(3, 1, 5), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 7), VGet(3, 1, 3), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(15, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 4: // Уровень 4 - телепорты! 🌀
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Телепорт А - вход (фиолетовый)
        blocks.push_back(Block(VGet(8, 1, 0), VGet(2, 1, 2), BlockType::TELEPORT, 0.0f, true, VGet(0, 0, 0), 1));

        // Платформа с телепортом Б
        blocks.push_back(Block(VGet(20, 3, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(21, 4, 1), VGet(2, 1, 2), BlockType::TELEPORT, 0.0f, true, VGet(0, 0, 0), 1));

        // Финишная платформа
        blocks.push_back(Block(VGet(30, 5, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(32, 6, 1), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 5: // Уровень 5 - ложные телепорты! 😈
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // 3 телепорта - только 1 правильный!
        blocks.push_back(Block(VGet(8, 1, -3), VGet(2, 1, 2), BlockType::TELEPORT, 0.0f, true, VGet(0, 0, 0), 1)); // В шипы!
        blocks.push_back(Block(VGet(8, 1, 0), VGet(2, 1, 2), BlockType::TELEPORT, 0.0f, true, VGet(0, 0, 0), 2));  // В никуда!
        blocks.push_back(Block(VGet(8, 1, 3), VGet(2, 1, 2), BlockType::TELEPORT, 0.0f, true, VGet(0, 0, 0), 3));  // Правильный!

        // Куда телепортируют:
        blocks.push_back(Block(VGet(20, 1, -3), VGet(3, 1, 3), BlockType::PLATFORM)); // Платформа с шипами
        blocks.push_back(Block(VGet(22, 2, -3), VGet(1, 1, 1), BlockType::SPIKES));   // Шипы!

        blocks.push_back(Block(VGet(20, 1, 3), VGet(3, 1, 3), BlockType::PLATFORM));  // Безопасная платформа
        blocks.push_back(Block(VGet(25, 1, 3), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 6: // Уровень 6 - кнопки и двери! 🔘
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Кнопка - делаем повыше чтобы точно сработала
        blocks.push_back(Block(VGet(8, 1, 0), VGet(2, 1, 2), BlockType::BUTTON, 0.0f, false, VGet(0, 0, 0), 10));

        // "Дверь" - платформа которая появляется при нажатии кнопки
        blocks.push_back(Block(VGet(15, 1, 0), VGet(4, 1, 4), BlockType::DISAPPEARING, 0.0f, false, VGet(0, 0, 0), 10));

        blocks.push_back(Block(VGet(22, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 7: // Уровень 7 - движущиеся платформы! 🚀
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Первая статичная платформа
        blocks.push_back(Block(VGet(5, 1, 0), VGet(3, 1, 3), BlockType::PLATFORM));

        // Движущаяся платформа туда-сюда по оси X
        blocks.push_back(Block(
            VGet(10, 2, 0),     // Начальная позиция
            VGet(4, 1, 4),      // Размер
            BlockType::MOVING,  // Тип
            0.0f,               // timer = 0
            true,               // isActive = true (направление вперед)
            VGet(20, 2, 0),     // moveTarget - конечная точка
            0                   // linkId
        ));

        // Вторая статичная платформа
        blocks.push_back(Block(VGet(25, 3, 0), VGet(3, 1, 3), BlockType::PLATFORM));

        // Финиш
        blocks.push_back(Block(VGet(30, 4, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 8: // Уровень 8 - исчезающие платформы! ⏱️
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Прыжок на первую исчезающую платформу
        blocks.push_back(Block(
            VGet(5, 2, 0),
            VGet(3, 1, 3),
            BlockType::DISAPPEARING,
            0.0f,      // timer = 0 (ещё не активирована)
            true,      // isActive = true (видна изначально)
            VGet(0, 0, 0),
            0          // linkId = 0 означает "исчезает после касания"
        ));

        // Вторая исчезающая платформа - нужно быстро прыгнуть!
        blocks.push_back(Block(
            VGet(10, 3, 0),
            VGet(3, 1, 3),
            BlockType::DISAPPEARING,
            0.0f,
            true,
            VGet(0, 0, 0),
            0
        ));

        // Третья - самая сложная, маленькая
        blocks.push_back(Block(
            VGet(15, 4, 0),
            VGet(2, 1, 2),
            BlockType::DISAPPEARING,
            0.0f,
            true,
            VGet(0, 0, 0),
            0
        ));

        // Безопасная платформа перед финишем
        blocks.push_back(Block(VGet(20, 5, 0), VGet(4, 1, 4), BlockType::PLATFORM));

        // Финиш
        blocks.push_back(Block(VGet(25, 5, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    default: // Дефолтный уровень
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
            // Невидимая стена - временно показываем красным каркасом для отладки
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
            break;

        case BlockType::FAKE_PLATFORM:
            // Выглядит как обычная платформа, но это обман!
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(180, 180, 180), GetColor(130, 130, 130), TRUE);
            break;

        case BlockType::SPIKES:
            // Красные шипы смерти!
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 50, 50), GetColor(200, 0, 0), TRUE);
            break;

        case BlockType::DISAPPEARING:
            // Мерцающая платформа - рисуем только если активна
            if (block.isActive) {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 255, 100), GetColor(200, 200, 50), TRUE);
            }
            break;

        case BlockType::MOVING:
            // Движущаяся платформа - зеленая
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
            break;

        case BlockType::BUTTON:
            // Кнопка - желтая если неактивна, зеленая если нажата
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
            // Фиолетовый телепорт
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 100, 255), GetColor(200, 50, 200), TRUE);
            break;
        }
    }
}

bool Level::CheckCollision(VECTOR playerPos, VECTOR playerSize, VECTOR& newPos, VECTOR& velocity, bool& onGround, VECTOR& platformVelocity) {
    onGround = false;
    platformVelocity = VGet(0, 0, 0); // Сбрасываем скорость платформы

    for (auto& block : blocks) {
        // Проверяем коллизии с твердыми блоками
        if (block.type == BlockType::PLATFORM ||
            block.type == BlockType::INVISIBLE_WALL ||
            block.type == BlockType::MOVING ||
            block.type == BlockType::BUTTON ||
            (block.type == BlockType::DISAPPEARING && block.isActive)) {

            // Проверяем пересечение AABB
            if (newPos.x < block.pos.x + block.size.x && newPos.x + playerSize.x > block.pos.x &&
                newPos.y < block.pos.y + block.size.y && newPos.y + playerSize.y > block.pos.y &&
                newPos.z < block.pos.z + block.size.z && newPos.z + playerSize.z > block.pos.z) {

                // Определяем с какой стороны столкнулись
                float overlapX = 0, overlapY = 0, overlapZ = 0;

                // Вычисляем пересечения по каждой оси
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

                // Исправляем столкновение по наименьшему пересечению
                if (overlapY <= overlapX && overlapY <= overlapZ) {
                    // Столкновение по Y (вертикаль)
                    if (velocity.y <= 0 && playerPos.y > block.pos.y + block.size.y - 1.5f) {
                        newPos.y = block.pos.y + block.size.y;
                        velocity.y = 0;
                        onGround = true;

                        // ВАЖНО: если стоим на движущейся платформе - передаём её скорость!
                        if (block.type == BlockType::MOVING) {
                            // Реальная скорость = разница между текущей и предыдущей позицией
                            platformVelocity = VGet(
                                block.pos.x - block.prevPos.x,
                                block.pos.y - block.prevPos.y,
                                block.pos.z - block.prevPos.z
                            );
                        }

                        // Активируем таймер исчезающей платформы при касании
                        if (block.type == BlockType::DISAPPEARING && block.linkId == 0) {
                            // linkId == 0 означает "исчезает после касания" (не привязана к кнопке)
                            if (block.timer == 0.0f) {
                                block.timer = 1.5f; // Начинаем отсчёт 1.5 секунды (быстрее!)
                            }
                        }
                    }
                    else if (velocity.y > 0) {
                        newPos.y = block.pos.y - playerSize.y;
                        velocity.y = 0;
                    }
                }
                else if (overlapX <= overlapZ) {
                    // Столкновение по X (лево-право)
                    if (newPos.x < block.pos.x) {
                        newPos.x = block.pos.x - playerSize.x;
                    }
                    else {
                        newPos.x = block.pos.x + block.size.x;
                    }
                }
                else {
                    // Столкновение по Z (вперед-назад)
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
            // Сохраняем позицию ДО движения
            block.prevPos = block.pos;

            // Движущаяся платформа: туда-сюда между originalPos и moveTarget
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

            // Интерполяция между originalPos и moveTarget
            float t = block.timer;
            block.pos.x = block.originalPos.x + (block.moveTarget.x - block.originalPos.x) * t;
            block.pos.y = block.originalPos.y + (block.moveTarget.y - block.originalPos.y) * t;
            block.pos.z = block.originalPos.z + (block.moveTarget.z - block.originalPos.z) * t;
        }

        if (block.type == BlockType::DISAPPEARING) {
            // Вариант Б: Исчезает после касания игроком
            if (block.linkId == 0 && block.timer > 0.0f) {
                block.timer -= deltaTime;

                if (block.timer <= 0.0f) {
                    // Время вышло - исчезаем!
                    block.isActive = false;
                    block.timer = -5.0f; // Исчезла на 5 секунд (отрицательное значение)
                }
            }

            // Возвращаем платформу обратно через 5 секунд
            if (block.linkId == 0 && block.timer < 0.0f) {
                block.timer += deltaTime;

                if (block.timer >= 0.0f) {
                    // Появляемся снова!
                    block.isActive = true;
                    block.timer = 0.0f; // Сбрасываем таймер
                }
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

                // Ищем телепорт-партнер с тем же linkId
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

                // Если партнер не найден - особые случаи по linkId
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