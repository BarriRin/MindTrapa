#include "Level.h"

Level::Level(int id) : levelId(id) {
    // Определить ID блока по уровню (10 уровней = 1 блок)
    blockId = (id - 1) / 10;

    LoadLevelData(id);
    InitializeModels();
}

Level::~Level() {
    // Удалить все дублированные модели блоков
    for (auto& block : blocks) {
        if (block.modelHandle != -1) {
            MV1DeleteModel(block.modelHandle);
        }
    }
    blocks.clear();
}

void Level::LoadLevelData(int id) {
    blocks.clear();

    if (id >= 1 && id <= 10) { LoadBlock1(id); return; }
    if (id >= 11 && id <= 20) { LoadBlock2(id); return; }
    if (id >= 21 && id <= 30) { LoadBlock3(id); return; }
    if (id >= 31 && id <= 40) { LoadBlock4(id); return; }
    if (id >= 41 && id <= 50) { LoadBlock5(id); return; }

    // Fallback for unknown levels
    playerSpawn = VGet(0, 1, 0);
    blocks.push_back(Block(VGet(-3, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));
    blocks.push_back(Block(VGet(5, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
}

void Level::Draw(bool debugMode) const {
    for (const auto& block : blocks) {
        // Если у блока есть модель — отрисовать модель (с учётом видимости)
        // Некоторые типы всегда используют примитивы (INVISIBLE_WALL, зоны)
        if (block.useModel && block.modelHandle != -1 &&
            block.type != BlockType::INVISIBLE_WALL &&
            block.type != BlockType::GRAVITY_ZONE &&
            block.type != BlockType::LIGHT_PULSE_ZONE) {

            // Проверка видимости для блоков с состоянием
            bool visible = true;
            if ((block.type == BlockType::DISAPPEARING || block.type == BlockType::CRUMBLING) && !block.isActive)
                visible = false;  // исчезли/упали — не рисуем
            if (block.type == BlockType::RETRACTABLE_SPIKES && !block.isActive)
                visible = false;  // шипы спрятаны — не рисуем
            if (block.type == BlockType::SPIKES && block.linkId == 1 && !block.isActive)
                visible = false;  // SPIKES(linkId=1) — выдвигающиеся, скрыты когда неактивны

            if (visible) {
                // Для моделей с origin в основании — ставим по нижней грани блока (X/Z центр, Y = низ)
                VECTOR modelPos;
                if (block.type == BlockType::TRIGGER)
                    modelPos = VGet(block.pos.x + block.size.x * 0.5f, block.pos.y, block.pos.z + block.size.z * 0.5f);
                else
                    modelPos = VAdd(block.pos, VScale(block.size, 0.5f));

                MV1SetPosition(block.modelHandle, modelPos);
                MV1SetScale(block.modelHandle, block.modelScale);
                MV1SetRotationXYZ(block.modelHandle, block.rotation);
                MV1DrawModel(block.modelHandle);

                // Hazard_Cylinder — полуцилиндр, рисуем зеркальную половину (180° по Y)
                if (block.type == BlockType::PENDULUM_BLADE) {
                    MV1SetRotationXYZ(block.modelHandle,
                        VGet(block.rotation.x, block.rotation.y + DX_PI_F, block.rotation.z));
                    MV1DrawModel(block.modelHandle);
                }
            }
            continue;
        }

        // Иначе использовать примитивы (текущая реализация)
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
            if (debugMode) {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
            }
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

        case BlockType::RETRACTABLE_SPIKES:
            // Выдвигающиеся шипы - видны только когда активны
            if (block.isActive) {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 50, 50), GetColor(200, 0, 0), TRUE);
            }
            // else: полностью невидимы когда спрятаны
            break;

        case BlockType::CRUMBLING:
            // Рассыпающиеся платформы - жёлто-коричневые
            if (block.isActive) {
                // Нормальное состояние
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(200, 150, 50), GetColor(150, 100, 30), TRUE);
            }
            else {
                // Падают/исчезают (полупрозрачные)
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(200, 150, 50), GetColor(150, 100, 30), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }
            break;

        case BlockType::FAKE_SPIKES:
            // Фейковые шипы - РОЗОВЫЕ (выглядят опасно, но безопасны)
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 100, 150), GetColor(200, 50, 100), TRUE);
            break;

        case BlockType::GRAVITY_ZONE:
            // Гравитационные зоны - очень прозрачные с цветом зависящим от типа гравитации
            unsigned int gravityColor;
            if (block.gravityMultiplier < 0.0f) {
                // Обратная гравитация - фиолетовый
                gravityColor = GetColor(200, 100, 255);
            } else if (block.gravityMultiplier < 1.0f) {
                // Низкая гравитация - голубой
                gravityColor = GetColor(100, 200, 255);
            } else if (block.gravityMultiplier > 1.0f) {
                // Высокая гравитация - оранжевый
                gravityColor = GetColor(255, 150, 50);
            } else {
                // Нормальная (не должно быть, но на всякий случай)
                gravityColor = GetColor(150, 150, 150);
            }
            // Отключаем запись в Z-буфер чтобы зона не перекрывала объекты внутри
            SetWriteZBuffer3D(FALSE);
            // Очень прозрачные - альфа 50 из 255 (~20% непрозрачности)
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                gravityColor, gravityColor, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            // Включаем запись в Z-буфер обратно
            SetWriteZBuffer3D(TRUE);
            break;

        case BlockType::PENDULUM_BLADE:
            // Маятник-топор - рисуем цепь (или трос) и лезвие
            // Цепь от точки подвеса до лезвия
            DrawLine3D(block.pivotPoint, block.pos, GetColor(100, 100, 100));

            // Лезвие - красный куб (временно, потом будет модель)
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(200, 50, 50), GetColor(150, 30, 30), TRUE);

            // Точка подвеса - маленькая сфера (упрощённо - куб)
            VECTOR pivotSize = VGet(0.3f, 0.3f, 0.3f);
            DrawCube3D(VGet(block.pivotPoint.x - 0.15f, block.pivotPoint.y - 0.15f, block.pivotPoint.z - 0.15f),
                VGet(block.pivotPoint.x + 0.15f, block.pivotPoint.y + 0.15f, block.pivotPoint.z + 0.15f),
                GetColor(80, 80, 80), GetColor(60, 60, 60), TRUE);
            break;

        case BlockType::LIGHT_PULSE_ZONE:
            // Зона мигающего света — очень прозрачная, чтобы игрок видел границы
            SetWriteZBuffer3D(FALSE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 25);
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(80, 60, 120), GetColor(80, 60, 120), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            SetWriteZBuffer3D(TRUE);
            break;

        case BlockType::ICE_PLATFORM:
            // Ледяная платформа — светло-голубая
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(180, 220, 255), GetColor(130, 180, 230), TRUE);
            break;

        case BlockType::BOUNCE_PAD:
            // Батут — жёлто-оранжевый
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 200, 50), GetColor(220, 150, 20), TRUE);
            break;
        }
    }
}

bool Level::CheckCollision(VECTOR playerPos, VECTOR playerSize, VECTOR& newPos, VECTOR& velocity, bool& onGround, VECTOR& platformVelocity) {
    onGround = false;
    platformVelocity = VGet(0, 0, 0);
    bool collisionOccurred = false;
    bool bounced = false; // флаг: батут уже сработал — пол не должен обнулять velocity.y

    for (auto& block : blocks) {
        if (block.type == BlockType::PLATFORM ||
            (block.type == BlockType::INVISIBLE_WALL && block.isActive) ||
            block.type == BlockType::MOVING ||
            block.type == BlockType::BUTTON ||
            (block.type == BlockType::DISAPPEARING && block.isActive) ||
            (block.type == BlockType::CRUMBLING && block.isActive) ||
            block.type == BlockType::ICE_PLATFORM ||
            block.type == BlockType::BOUNCE_PAD) {

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

                        if (block.type == BlockType::BOUNCE_PAD) {
                            if (!bounced) { velocity.y = 0.6f; bounced = true; }
                        } else {
                            if (!bounced) { velocity.y = 0; onGround = true; }
                        }

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

                        if (block.type == BlockType::CRUMBLING) {
                            if (block.timer == 0.0f) {
                                block.timer = 0.5f * block.moveSpeed;
                            }
                        }
                    }
                    else if (velocity.y > 0) {
                        newPos.y = block.pos.y - playerSize.y;
                        if (!bounced) velocity.y = 0;
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

                collisionOccurred = true;
                // Продолжаем цикл — батуты на платформах должны тоже проверяться
            }
        }
    }

    return collisionOccurred;
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
        // Обычные шипы
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

        // Выдвижные шипы - убивают только когда активны
        if (block.type == BlockType::RETRACTABLE_SPIKES && block.isActive) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return true;
            }
        }

        // Маятники-топоры (Block 3) - всегда смертельны
        if (block.type == BlockType::PENDULUM_BLADE) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return true;
            }
        }

        // FAKE_SPIKES НЕ убивают - это троллинг!
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

        // RETRACTABLE_SPIKES - выдвигаются/прячутся по таймеру
        if (block.type == BlockType::RETRACTABLE_SPIKES) {
            block.timer += deltaTime;

            // speed=1.0 → 4s цикл (2s вверх + 2s вниз), speed=2.0 → 2s цикл
            float cycleTime = 4.0f / block.moveSpeed;
            float halfCycle = cycleTime / 2.0f;
            float phase = fmod(block.timer, cycleTime);

            block.isActive = (phase < halfCycle); // true = шипы выдвинуты (опасно)
        }

        // CRUMBLING - платформы рассыпаются после касания игрока
        // (логика касания будет в CheckCollision)
        if (block.type == BlockType::CRUMBLING) {
            // Если платформа начала рассыпаться (timer > 0)
            if (block.timer > 0.0f) {
                block.timer -= deltaTime;

                // Через 0.5 сек платформа исчезает
                if (block.timer <= 0.0f) {
                    block.isActive = false; // Платформа упала
                    block.timer = -2.0f;    // Время до возрождения
                }
            }

            // Платформа исчезла, ждём возрождения
            if (block.timer < 0.0f) {
                block.timer += deltaTime;

                // Через 2 сек платформа возвращается
                if (block.timer >= 0.0f) {
                    block.isActive = true;
                    block.timer = 0.0f;
                }
            }
        }

        // LIGHT_PULSE_ZONE - мигающий свет [Block 4]
        // moveSpeed = период цикла (сек), isActive = true когда светло
        if (block.type == BlockType::LIGHT_PULSE_ZONE) {
            block.timer += deltaTime;
            if (block.timer >= block.moveSpeed) {
                block.timer -= block.moveSpeed;
            }
            block.isActive = (block.timer < block.moveSpeed * 0.5f); // первая половина = светло
        }

        // PENDULUM_BLADE - качающиеся маятники (Block 3)
        if (block.type == BlockType::PENDULUM_BLADE) {
            // Обновляем угол качания используя синусоиду
            block.swingAngle = sin(block.timer * block.swingSpeed) * block.swingRange;

            // Обновляем таймер
            block.timer += deltaTime;

            // Вычисляем длину маятника как расстояние от точки подвеса до начальной позиции
            float length = VSize(VSub(block.pivotPoint, block.originalPos));

            // Вычисляем позицию лезвия на основе угла качания
            block.pos.x = block.pivotPoint.x + sin(block.swingAngle) * length;
            block.pos.y = block.pivotPoint.y - cos(block.swingAngle) * length;
            block.pos.z = block.pivotPoint.z;
        }
    }
}

bool Level::CheckTeleportTrigger(VECTOR playerPos, VECTOR playerSize, VECTOR& teleportTarget) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::TELEPORT) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {

                // linkId=0: неправильный выбор — возврат на спавн уровня
                if (block.linkId == 0) {
                    teleportTarget = VGet(playerSpawn.x + 0.5f, playerSpawn.y + 2.0f, playerSpawn.z + 0.5f);
                    return true;
                }

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

                if (block.linkId != 0) {
                    for (auto& otherBlock : blocks) {
                        if (otherBlock.linkId == block.linkId && otherBlock.type == BlockType::DISAPPEARING) {
                            otherBlock.isActive = block.isActive;
                        }
                        if (otherBlock.linkId == block.linkId && otherBlock.type == BlockType::INVISIBLE_WALL) {
                            otherBlock.isActive = block.isActive;
                        }
                    }
                }
            }
        }
    }
}

bool Level::IsAnyLightPulseZoneActive() const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::LIGHT_PULSE_ZONE && block.isActive)
            return true;
    }
    return false;
}

void Level::InitializeModels() {
    ModelManager& modelMgr = ModelManager::GetInstance();

    for (auto& block : blocks) {
        // Определить ModelID по BlockType
        ModelID modelId = static_cast<ModelID>(static_cast<int>(block.type));

        // Если модель загружена для данного типа блока
        if (modelMgr.IsModelLoaded(modelId)) {
            // Создать дубликат модели для этого блока
            block.modelHandle = modelMgr.DuplicateModel(modelId);
            block.useModel = true;

            // Масштаб: делим на нативный размер модели (~2.0 для Quaternius pack)
            // MV1SetScale — это множитель, не абсолютный размер
            const float NATIVE = 2.0f;
            block.modelScale = VGet(
                block.size.x / NATIVE,
                block.size.y / NATIVE,
                block.size.z / NATIVE
            );

            // Флаг (TRIGGER): повернуть лицом к игроку
            if (block.type == BlockType::TRIGGER)
                block.rotation = VGet(0, DX_PI_F * 0.5f, 0);
            // Цилиндр маятника: модель стоит вертикально, укладываем вдоль Z (90° по X)
            if (block.type == BlockType::PENDULUM_BLADE)
                block.rotation = VGet(DX_PI_F * 0.5f, 0, 0);
        }
        else {
            // Модель не загружена - будут использоваться примитивы
            block.modelHandle = -1;
            block.useModel = false;
        }
    }
}

void Level::DrawSkybox(VECTOR cameraPos) const {
    ModelManager& modelMgr = ModelManager::GetInstance();

    // Отрисовать скайбокс для текущего блока уровней
    if (modelMgr.IsSkyboxLoaded(blockId)) {
        modelMgr.DrawSkybox(blockId, cameraPos);
    }
}

// Block 3: Проверка гравитационных зон
float Level::CheckGravityZone(VECTOR playerPos, VECTOR playerSize) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::GRAVITY_ZONE) {
            // Проверка нахождения игрока в зоне
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return block.gravityMultiplier; // Возвращаем множитель гравитации
            }
        }
    }
    return 1.0f; // Нормальная гравитация по умолчанию
}

float Level::CheckIcePlatform(VECTOR playerPos, VECTOR playerSize) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::ICE_PLATFORM) {
            float blockTop = block.pos.y + block.size.y;
            // Игрок стоит на льду: ноги (playerPos.y) у верхней грани блока + X/Z перекрытие
            if (playerPos.y >= blockTop - 0.15f && playerPos.y <= blockTop + 0.15f &&
                playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return block.frictionMultiplier;
            }
        }
    }
    return 1.0f; // Не на льду — нормальное управление
}