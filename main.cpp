#include "DxLib.h"
#include <cmath>

// Структура для блока
struct Block {
    VECTOR pos;
    VECTOR size;
    int type; // 0-пол, 1-триггер
};

// Данные уровней
int currentLevel = 1;
int totalLevels = 3;
int deaths = 0;

// Уровень 1 - простой туториал (ближе к игроку)
Block level1[] = {
    {{-5, -1, -5}, {10, 1, 10}, 0},       // платформа под игроком
    {{8, 1, 0}, {3, 1, 3}, 0},            // платформа для прыжка
    {{15, 3, 0}, {2, 1, 2}, 1}            // триггер-финиш
};
int level1_count = 3;

// Уровень 2 - прыжки
Block level2[] = {
    {{-10, -2, -10}, {10, 2, 10}, 0},     // стартовая платформа
    {{5, 0, -5}, {5, 1, 5}, 0},           // средняя платформа
    {{15, 2, 0}, {5, 1, 5}, 0},           // высокая платформа
    {{25, 2, 0}, {3, 1, 3}, 1}            // финиш
};
int level2_count = 4;

// Уровень 3 - первый троллинг!
Block level3[] = {
    {{-10, -2, -10}, {8, 2, 8}, 0},       // стартовая платформа
    {{5, 0, -5}, {3, 1, 3}, 0},           // обманная средняя платформа
    {{15, 5, 0}, {3, 1, 3}, 0},           // высокая платформа (кажется что финиш)
    {{10, -2, 5}, {5, 1, 5}, 0},          // скрытая платформа
    {{20, -2, 5}, {3, 1, 3}, 1}           // настоящий финиш (внизу!)
};
int level3_count = 5;

// Текущие блоки уровня
Block* currentBlocks;
int currentBlockCount;

// Функция загрузки уровня
void LoadLevel(int levelNum) {
    currentLevel = levelNum;

    switch (levelNum) {
    case 1:
        currentBlocks = level1;
        currentBlockCount = level1_count;
        break;
    case 2:
        currentBlocks = level2;
        currentBlockCount = level2_count;
        break;
    case 3:
        currentBlocks = level3;
        currentBlockCount = level3_count;
        break;
    default:
        currentLevel = 1;
        LoadLevel(1);
        break;
    }
}

// Функция перехода на следующий уровень
void NextLevel() {
    currentLevel++;
    if (currentLevel > totalLevels) {
        currentLevel = 1; // Перезапуск игры
    }
    LoadLevel(currentLevel);
}

// Функция рестарта уровня
void RestartLevel() {
    deaths++;
    LoadLevel(currentLevel);
}

// Простая проверка коллизии AABB
bool CheckCollision(VECTOR pos1, VECTOR size1, VECTOR pos2, VECTOR size2) {
    return (pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x &&
        pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y &&
        pos1.z < pos2.z + size2.z && pos1.z + size1.z > pos2.z);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(FALSE); // Полноэкранный режим как было
    SetGraphMode(1920, 1080, 32);
    if (DxLib_Init() == -1) return -1;

    // Полная настройка 3D
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    SetUseBackCulling(TRUE);
    SetUseLighting(FALSE);
    SetDrawScreen(DX_SCREEN_BACK);

    // Настройка камеры
    SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
    SetCameraNearFar(0.1f, 1000.0f);

    // Инициализация
    LoadLevel(1);

    // Позиция и физика игрока
    VECTOR playerPos = VGet(0, 1, 0);  // Стартуем над платформой
    VECTOR playerVel = VGet(0, 0, 0);
    VECTOR playerSize = VGet(1, 1, 1);

    // Камера (твоя старая система)
    float cameraAngleX = 0.0f;
    float cameraAngleY = 0.0f;
    float cameraDistance = 10.0f;

    // Параметры движения
    float speed = 0.2f;
    float jumpPower = 0.4f;
    float gravity = -0.015f;
    bool onGround = false;

    // Скрываем курсор мыши
    SetMouseDispFlag(FALSE);

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        ClearDrawScreen();

        // === УПРАВЛЕНИЕ КАМЕРОЙ МЫШЬЮ (твоя оригинальная система) ===
        int mouseX, mouseY;
        GetMousePoint(&mouseX, &mouseY);

        int centerX = 1920 / 2;
        int centerY = 1080 / 2;

        cameraAngleY += (mouseX - centerX) * 0.002f;
        cameraAngleX += (mouseY - centerY) * 0.002f;

        // Ограничения для камеры
        if (cameraAngleX > 1.5f) cameraAngleX = 1.5f;
        if (cameraAngleX < -1.5f) cameraAngleX = -1.5f;

        SetMousePoint(centerX, centerY);

        // === УПРАВЛЕНИЕ ИГРОКОМ ===
        VECTOR move = VGet(0, 0, 0);

        // Правильное движение относительно камеры
        VECTOR forward = VGet(sin(cameraAngleY), 0, cos(cameraAngleY));
        VECTOR right = VGet(cos(cameraAngleY), 0, -sin(cameraAngleY));

        if (CheckHitKey(KEY_INPUT_W)) move = VAdd(move, VScale(forward, -speed));  // Поменял знак
        if (CheckHitKey(KEY_INPUT_S)) move = VAdd(move, VScale(forward, speed));   // Поменял знак
        if (CheckHitKey(KEY_INPUT_A)) move = VAdd(move, VScale(right, speed));
        if (CheckHitKey(KEY_INPUT_D)) move = VAdd(move, VScale(right, -speed));

        // Прыжок
        static bool jumpPressed = false;
        if (CheckHitKey(KEY_INPUT_SPACE)) {
            if (!jumpPressed && onGround) {
                playerVel.y = jumpPower;
                onGround = false;
            }
            jumpPressed = true;
        }
        else {
            jumpPressed = false;
        }

        // Рестарт уровня на R
        static bool restartPressed = false;
        if (CheckHitKey(KEY_INPUT_R)) {
            if (!restartPressed) {
                RestartLevel();
                playerPos = VGet(0, 1, 0);  // Над платформой
                playerVel = VGet(0, 0, 0);
            }
            restartPressed = true;
        }
        else {
            restartPressed = false;
        }

        // === ФИЗИКА ===
        playerVel.y += gravity;

        // Применяем движение по X и Z
        playerPos.x += move.x;
        playerPos.z += move.z;

        // Применяем гравитацию
        playerPos.y += playerVel.y;

        // === КОЛЛИЗИИ С БЛОКАМИ ===
        onGround = false;
        for (int i = 0; i < currentBlockCount; i++) {
            Block& block = currentBlocks[i];

            if (CheckCollision(playerPos, playerSize, block.pos, block.size)) {
                if (block.type == 1) { // Триггер
                    NextLevel();
                    playerPos = VGet(0, 1, 0);  // Над платформой
                    playerVel = VGet(0, 0, 0);
                    break;
                }
                else { // Обычная платформа
                    // Простая коллизия - ставим игрока на верх блока
                    if (playerVel.y <= 0 && playerPos.y > block.pos.y + block.size.y - 1.5f) {
                        playerPos.y = block.pos.y + block.size.y;
                        playerVel.y = 0;
                        onGround = true;
                    }
                }
            }
        }

        // Проверка падения в пропасть
        if (playerPos.y < -10) {
            RestartLevel();
            playerPos = VGet(0, 1, 0);  // Над платформой
            playerVel = VGet(0, 0, 0);
        }

        // === УСТАНОВКА КАМЕРЫ ===
        VECTOR cameraPos = VGet(
            playerPos.x + cameraDistance * sin(cameraAngleY) * cos(cameraAngleX),
            playerPos.y + cameraDistance * sin(cameraAngleX) + 2.0f, // Поднимаем камеру немного выше
            playerPos.z + cameraDistance * cos(cameraAngleY) * cos(cameraAngleX)
        );

        SetCameraPositionAndTarget_UpVecY(cameraPos, VAdd(playerPos, VGet(0, 1, 0))); // Смотрим немного выше центра игрока

        // === ОТРИСОВКА ===
        SetUseLighting(FALSE);
        SetUseBackCulling(FALSE);

        // Блоки
        for (int i = 0; i < currentBlockCount; i++) {
            Block& block = currentBlocks[i];

            if (block.type == 0) { // Обычная платформа
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(200, 200, 200), GetColor(150, 150, 150), TRUE);
            }
            else if (block.type == 1) { // Триггер
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(100, 200, 255), GetColor(50, 150, 200), TRUE);
            }
        }

        // Игрок
        DrawCube3D(playerPos, VAdd(playerPos, playerSize),
            GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);

        // UI + отладочная информация
        DrawFormatString(10, 10, GetColor(255, 255, 255), L"Level: %d/%d", currentLevel, totalLevels);
        DrawFormatString(10, 30, GetColor(255, 255, 255), L"Deaths: %d", deaths);
        DrawFormatString(10, 50, GetColor(255, 255, 255), L"WASD - move, Space - jump, R - restart");
        DrawFormatString(10, 70, GetColor(255, 255, 255), L"Get to blue block!");

        // Отладочная информация
        DrawFormatString(10, 100, GetColor(255, 255, 0), L"Player: X=%.1f Y=%.1f Z=%.1f", playerPos.x, playerPos.y, playerPos.z);
        DrawFormatString(10, 120, GetColor(255, 255, 0), L"Camera: AngleX=%.1f AngleY=%.1f", cameraAngleX, cameraAngleY);
        DrawFormatString(10, 140, GetColor(255, 255, 0), L"Blocks in level: %d", currentBlockCount);

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}