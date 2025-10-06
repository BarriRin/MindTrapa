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

    // Система контроля скорости игры
    float gameSpeed = 1.0f; // 1.0 = нормальная скорость
    float targetDeltaTime = 0.016f; // Целевой deltaTime для 60 FPS

    // Антиспам для телепортов
    float teleportCooldown = 0.0f;

    // Скрываем курсор мыши
    SetMouseDispFlag(FALSE);

    // === ОСНОВНОЙ ИГРОВОЙ ЦИКЛ ===
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        ClearDrawScreen();

        // Вычисляем эффективный deltaTime с учётом скорости игры
        float effectiveDeltaTime = targetDeltaTime * gameSpeed;

        // Уменьшаем кулдаун телепорта
        if (teleportCooldown > 0) {
            teleportCooldown -= effectiveDeltaTime;
        }

        // === УПРАВЛЕНИЕ СКОРОСТЬЮ ИГРЫ ===
        static bool plusPressed = false;
        static bool minusPressed = false;

        // Клавиша + (несколько вариантов для разных клавиатур)
        if (CheckHitKey(KEY_INPUT_ADD) || CheckHitKey(KEY_INPUT_SEMICOLON) || CheckHitKey(KEY_INPUT_COLON)) {
            if (!plusPressed) {
                gameSpeed += 0.25f;
                if (gameSpeed > 3.0f) gameSpeed = 3.0f;
            }
            plusPressed = true;
        }
        else {
            plusPressed = false;
        }

        // Клавиша - (несколько вариантов)
        if (CheckHitKey(KEY_INPUT_SUBTRACT) || CheckHitKey(KEY_INPUT_MINUS)) {
            if (!minusPressed) {
                gameSpeed -= 0.25f;
                if (gameSpeed < 0.25f) gameSpeed = 0.25f;
            }
            minusPressed = true;
        }
        else {
            minusPressed = false;
        }

        // Альтернатива: клавиши [ и ] (более универсальные)
        static bool bracketLeftPressed = false;
        static bool bracketRightPressed = false;

        if (CheckHitKey(KEY_INPUT_LBRACKET)) { // [
            if (!bracketLeftPressed) {
                gameSpeed -= 0.25f;
                if (gameSpeed < 0.25f) gameSpeed = 0.25f;
            }
            bracketLeftPressed = true;
        }
        else {
            bracketLeftPressed = false;
        }

        if (CheckHitKey(KEY_INPUT_RBRACKET)) { // ]
            if (!bracketRightPressed) {
                gameSpeed += 0.25f;
                if (gameSpeed > 3.0f) gameSpeed = 3.0f;
            }
            bracketRightPressed = true;
        }
        else {
            bracketRightPressed = false;
        }

        // Клавиша 0 - сброс скорости
        static bool zeroPressed = false;
        if (CheckHitKey(KEY_INPUT_0) || CheckHitKey(KEY_INPUT_NUMPAD0)) {
            if (!zeroPressed) {
                gameSpeed = 1.0f;
            }
            zeroPressed = true;
        }
        else {
            zeroPressed = false;
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

        // Движение относительно камеры (с учётом deltaTime!)
        VECTOR forward = VGet(sin(cameraAngleY), 0, cos(cameraAngleY));
        VECTOR right = VGet(cos(cameraAngleY), 0, -sin(cameraAngleY));

        // Применяем скорость с учётом времени
        float moveSpeed = speed * (effectiveDeltaTime / 0.016f); // Нормализуем к 60 FPS

        if (CheckHitKey(KEY_INPUT_W)) move = VAdd(move, VScale(forward, -moveSpeed));
        if (CheckHitKey(KEY_INPUT_S)) move = VAdd(move, VScale(forward, moveSpeed));
        if (CheckHitKey(KEY_INPUT_A)) move = VAdd(move, VScale(right, moveSpeed));
        if (CheckHitKey(KEY_INPUT_D)) move = VAdd(move, VScale(right, -moveSpeed));

        // Прыжок (импульс - не зависит от deltaTime напрямую)
        static bool jumpPressed = false;
        if (CheckHitKey(KEY_INPUT_SPACE)) {
            if (!jumpPressed && onGround) {
                // Прыжок должен компенсировать gameSpeed для постоянной высоты
                playerVel.y = jumpPower * gameSpeed;
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
        // Гравитация с учётом gameSpeed (но не FPS - уже нормализовано)
        playerVel.y += gravity * gameSpeed;

        // Новая позиция с учетом всего движения
        VECTOR newPos = VAdd(playerPos, VAdd(move, VGet(0, playerVel.y, 0)));

        // === КОЛЛИЗИИ ===
        Level* currentLevel = levelManager.GetCurrentLevel();
        if (currentLevel) {
            // Обновляем динамические элементы (движущиеся платформы) с учётом скорости игры
            currentLevel->Update(effectiveDeltaTime);

            // Проверка коллизий с движущимися платформами
            VECTOR platformVelocity = VGet(0, 0, 0);
            currentLevel->CheckCollision(playerPos, playerSize, newPos, playerVel, onGround, platformVelocity);

            // Применяем скорость платформы к игроку (если стоит на ней)
            newPos = VAdd(newPos, platformVelocity);

            // Применяем исправленную позицию
            playerPos = newPos;

            // Проверка шипов - СМЕРТЬ!
            if (currentLevel->CheckDeadlyTrigger(playerPos, playerSize)) {
                levelManager.RestartLevel();
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
            }

            // Проверка телепортов!
            VECTOR teleportTarget;
            if (teleportCooldown <= 0 && currentLevel->CheckTeleportTrigger(playerPos, playerSize, teleportTarget)) {
                playerPos = teleportTarget;
                playerVel = VGet(0, 0, 0); // Сбрасываем скорость после телепорта
                teleportCooldown = 1.0f;   // Кулдаун 1 секунда
            }

            // Проверка кнопок!
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

        // Отладочная информация + индикатор скорости игры
        DrawFormatString(10, 140, GetColor(255, 255, 0), L"Player: X=%.1f Y=%.1f Z=%.1f",
            playerPos.x, playerPos.y, playerPos.z);

        // Показываем текущую скорость игры
        unsigned int speedColor = GetColor(100, 255, 100);
        if (gameSpeed > 1.0f) speedColor = GetColor(255, 200, 100); // Оранжевый если быстрее
        if (gameSpeed < 1.0f) speedColor = GetColor(100, 200, 255); // Голубой если медленнее
        DrawFormatString(10, 160, speedColor, L"Game Speed: %.2fx ([ ] to change, 0 to reset)", gameSpeed);

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}