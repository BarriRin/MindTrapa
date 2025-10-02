#include "DxLib.h"
#include "LevelManager.h"
#include <cmath>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // Инициализация DxLib
    ChangeWindowMode(FALSE);
    SetGraphMode(1920, 1080, 32);
    if (DxLib_Init() == -1) return -1;

    // Настройка 3D
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    SetUseBackCulling(TRUE);
    SetUseLighting(FALSE);
    SetDrawScreen(DX_SCREEN_BACK);

    // Настройка камеры
    SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
    SetCameraNearFar(0.1f, 1000.0f);

    // Инициализация игры
    LevelManager levelManager;

    // Игрок
    VECTOR playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
    VECTOR playerVel = VGet(0, 0, 0);
    VECTOR playerSize = VGet(1, 1, 1);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
    // Камера
    float cameraAngleX = 0.0f;
    float cameraAngleY = 0.0f;
    float cameraDistance = 10.0f;

    // Параметры движения
    float speed = 0.2f;
    float jumpPower = 0.4f;
    float gravity = -0.015f;
    bool onGround = false;

    // Антиспам для телепортов
    float teleportCooldown = 0.0f;

    // Скрываем курсор мыши
    SetMouseDispFlag(FALSE);

    // === ОСНОВНОЙ ИГРОВОЙ ЦИКЛ ===
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        ClearDrawScreen();

        // Уменьшаем кулдаун телепорта
        if (teleportCooldown > 0) {
            teleportCooldown -= 0.016f; // ~60 FPS
        }

        // === УПРАВЛЕНИЕ КАМЕРОЙ МЫШЬЮ ===
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

        // Движение относительно камеры
        VECTOR forward = VGet(sin(cameraAngleY), 0, cos(cameraAngleY));
        VECTOR right = VGet(cos(cameraAngleY), 0, -sin(cameraAngleY));

        if (CheckHitKey(KEY_INPUT_W)) move = VAdd(move, VScale(forward, -speed));
        if (CheckHitKey(KEY_INPUT_S)) move = VAdd(move, VScale(forward, speed));
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

        // Кнопка активации E
        static bool activatePressed = false;
        bool activateKeyPressed = false;
        if (CheckHitKey(KEY_INPUT_E)) {
            if (!activatePressed) {
                activateKeyPressed = true;
            }
            activatePressed = true;
        }
        else {
            activatePressed = false;
        }

        // Рестарт уровня на R
        static bool restartPressed = false;
        if (CheckHitKey(KEY_INPUT_R)) {
            if (!restartPressed) {
                levelManager.RestartLevel();
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
            }
            restartPressed = true;
        }
        else {
            restartPressed = false;
        }

        // === ФИЗИКА ===
        playerVel.y += gravity;

        // Новая позиция с учетом всего движения
        VECTOR newPos = VAdd(playerPos, VAdd(move, VGet(0, playerVel.y, 0)));

        // === КОЛЛИЗИИ ===
        Level* currentLevel = levelManager.GetCurrentLevel();
        if (currentLevel) {
            // Проверка коллизий - теперь исправляет newPos по всем осям!
            currentLevel->CheckCollision(playerPos, playerSize, newPos, playerVel, onGround);

            // Применяем исправленную позицию
            playerPos = newPos;

            // Проверка шипов - СМЕРТЬ! 💀
            if (currentLevel->CheckDeadlyTrigger(playerPos, playerSize)) {
                levelManager.RestartLevel();
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
            }

            // Проверка телепортов! 🌀
            VECTOR teleportTarget;
            if (teleportCooldown <= 0 && currentLevel->CheckTeleportTrigger(playerPos, playerSize, teleportTarget)) {
                playerPos = teleportTarget;
                playerVel = VGet(0, 0, 0); // Сбрасываем скорость после телепорта
                teleportCooldown = 1.0f;   // Кулдаун 1 секунда
            }

            // Проверка кнопок! 🔘
            currentLevel->ActivateButton(playerPos, playerSize, activateKeyPressed);

            // Проверка триггера победы
            if (currentLevel->CheckWinTrigger(playerPos, playerSize)) {
                levelManager.NextLevel();
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
            }
        }

        // Проверка падения в пропасть
        if (playerPos.y < -10) {
            levelManager.RestartLevel();
            playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
            playerVel = VGet(0, 0, 0);
        }

        // === УСТАНОВКА КАМЕРЫ ===
        VECTOR cameraPos = VGet(
            playerPos.x + cameraDistance * sin(cameraAngleY) * cos(cameraAngleX),
            playerPos.y + cameraDistance * sin(cameraAngleX) + 2.0f,
            playerPos.z + cameraDistance * cos(cameraAngleY) * cos(cameraAngleX)
        );

        SetCameraPositionAndTarget_UpVecY(cameraPos, VAdd(playerPos, VGet(0, 1, 0)));

        // === ОТРИСОВКА ===
        SetUseLighting(FALSE);
        SetUseBackCulling(FALSE);

        // Отрисовка уровня
        if (levelManager.GetCurrentLevel()) {
            levelManager.GetCurrentLevel()->Draw();
        }

        // Игрок
        DrawCube3D(playerPos, VAdd(playerPos, playerSize),
            GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);

        // UI
        levelManager.DrawLevelInfo();

        // Отладочная информация
        DrawFormatString(10, 120, GetColor(255, 255, 0), L"Player: X=%.1f Y=%.1f Z=%.1f",
            playerPos.x, playerPos.y, playerPos.z);

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}