#include "DxLib.h"
#include "LevelManager.h"
#include "Menu.h"
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
    Menu menu(10); // 10 уровней - первый блок завершён!

    // ОТЛАДКА - проверяем что меню создалось правильно
    char debug[100];
    sprintf_s(debug, "Menu created with 10 levels");
    OutputDebugStringA(debug);

    // Состояние игры
    GameState gameState = GameState::MAIN_MENU;

    // Игрок
    VECTOR playerPos = VGet(0, 0, 0);
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
    float gameSpeed = 1.0f;
    float targetDeltaTime = 0.016f;

    // Антиспам для телепортов
    float teleportCooldown = 0.0f;

    // Скрываем курсор мыши только в игре
    SetMouseDispFlag(TRUE);

    // === ОСНОВНОЙ ИГРОВОЙ ЦИКЛ ===
    while (ProcessMessage() == 0) {
        ClearDrawScreen();

        // Вычисляем эффективный deltaTime с учётом скорости игры
        float effectiveDeltaTime = targetDeltaTime * gameSpeed;

        // === ОБРАБОТКА В ЗАВИСИМОСТИ ОТ СОСТОЯНИЯ ===
        if (gameState == GameState::MAIN_MENU || gameState == GameState::LEVEL_SELECT) {
            // Показываем курсор в меню
            SetMouseDispFlag(TRUE);

            // Отрисовка меню
            menu.SetState(gameState);
            menu.Draw();

            // Обработка ввода в меню паузы
            int levelToLoad = -1;
            ButtonAction action = menu.HandleInput(levelToLoad);

            // ESC для возврата в игру из паузы (отдельная обработка)
            static bool pauseEscPressed = false;
            if (CheckHitKey(KEY_INPUT_ESCAPE)) {
                if (!pauseEscPressed) {
                    gameState = GameState::PLAYING;
                    SetMouseDispFlag(FALSE);
                }
                pauseEscPressed = true;
            }
            else {
                pauseEscPressed = false;
            }

            switch (action) {
            case ButtonAction::START_GAME:
                levelManager.LoadLevel(1);
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
                gameState = GameState::PLAYING;
                SetMouseDispFlag(FALSE);
                break;

            case ButtonAction::LEVEL_SELECT:
                gameState = GameState::LEVEL_SELECT;
                break;

            case ButtonAction::EXIT_GAME:
                goto END_GAME; // Выход из игры

            case ButtonAction::BACK_TO_MENU:
                gameState = GameState::MAIN_MENU;
                break;

            default:
                // Загрузка конкретного уровня
                if (levelToLoad > 0) {
                    levelManager.LoadLevel(levelToLoad);
                    playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                    playerVel = VGet(0, 0, 0);
                    gameState = GameState::PLAYING;
                    SetMouseDispFlag(FALSE);
                }
                break;
            }
        }
        else if (gameState == GameState::PAUSED) {
            // Показываем курсор в меню
            SetMouseDispFlag(TRUE);

            // Рисуем игру на фоне
            Level* currentLevel = levelManager.GetCurrentLevel();
            if (currentLevel) {
                // Установка камеры
                VECTOR cameraPos = VGet(
                    playerPos.x + cameraDistance * sin(cameraAngleY) * cos(cameraAngleX),
                    playerPos.y + cameraDistance * sin(cameraAngleX) + 2.0f,
                    playerPos.z + cameraDistance * cos(cameraAngleY) * cos(cameraAngleX)
                );
                SetCameraPositionAndTarget_UpVecY(cameraPos, VAdd(playerPos, VGet(0, 1, 0)));

                SetUseLighting(FALSE);
                SetUseBackCulling(FALSE);
                currentLevel->Draw();
                DrawCube3D(playerPos, VAdd(playerPos, playerSize),
                    GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
            }

            // Рисуем меню паузы поверх
            menu.SetState(GameState::PAUSED);
            menu.Draw();

            // Обработка ввода
            int levelToLoad = -1;
            ButtonAction action = menu.HandleInput(levelToLoad);

            switch (action) {
            case ButtonAction::CONTINUE:
                gameState = GameState::PLAYING;
                SetMouseDispFlag(FALSE);
                break;

            case ButtonAction::RESTART:
                levelManager.RestartLevel();
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
                gameState = GameState::PLAYING;
                SetMouseDispFlag(FALSE);
                break;

            case ButtonAction::LEVEL_SELECT:
                gameState = GameState::LEVEL_SELECT;
                SetMouseDispFlag(TRUE);
                break;

            case ButtonAction::BACK_TO_MENU:
                gameState = GameState::MAIN_MENU;
                SetMouseDispFlag(TRUE);
                break;
            }
        }
        else if (gameState == GameState::PLAYING) {
            // Скрываем курсор
            SetMouseDispFlag(FALSE);

            // ESC - пауза (с задержкой чтобы не закрывалось сразу)
            static bool escPressed = false;
            static float escDelay = 0.0f;

            if (escDelay > 0.0f) {
                escDelay -= effectiveDeltaTime;
            }

            if (CheckHitKey(KEY_INPUT_ESCAPE)) {
                if (!escPressed && escDelay <= 0.0f) {
                    gameState = GameState::PAUSED;
                    SetMouseDispFlag(TRUE);
                    escDelay = 0.3f; // Задержка 0.3 секунды
                }
                escPressed = true;
            }
            else {
                escPressed = false;
            }

            // Уменьшаем кулдаун телепорта
            if (teleportCooldown > 0) {
                teleportCooldown -= effectiveDeltaTime;
            }

            // === УПРАВЛЕНИЕ СКОРОСТЬЮ ИГРЫ ===
            static bool plusPressed = false;
            static bool minusPressed = false;

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

            static bool bracketLeftPressed = false;
            static bool bracketRightPressed = false;

            if (CheckHitKey(KEY_INPUT_LBRACKET)) {
                if (!bracketLeftPressed) {
                    gameSpeed -= 0.25f;
                    if (gameSpeed < 0.25f) gameSpeed = 0.25f;
                }
                bracketLeftPressed = true;
            }
            else {
                bracketLeftPressed = false;
            }

            if (CheckHitKey(KEY_INPUT_RBRACKET)) {
                if (!bracketRightPressed) {
                    gameSpeed += 0.25f;
                    if (gameSpeed > 3.0f) gameSpeed = 3.0f;
                }
                bracketRightPressed = true;
            }
            else {
                bracketRightPressed = false;
            }

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

            if (cameraAngleX > 1.5f) cameraAngleX = 1.5f;
            if (cameraAngleX < -1.5f) cameraAngleX = -1.5f;

            SetMousePoint(centerX, centerY);

            // === УПРАВЛЕНИЕ ИГРОКОМ ===
            VECTOR move = VGet(0, 0, 0);

            VECTOR forward = VGet(sin(cameraAngleY), 0, cos(cameraAngleY));
            VECTOR right = VGet(cos(cameraAngleY), 0, -sin(cameraAngleY));

            float moveSpeed = speed * (effectiveDeltaTime / 0.016f);

            if (CheckHitKey(KEY_INPUT_W)) move = VAdd(move, VScale(forward, -moveSpeed));
            if (CheckHitKey(KEY_INPUT_S)) move = VAdd(move, VScale(forward, moveSpeed));
            if (CheckHitKey(KEY_INPUT_A)) move = VAdd(move, VScale(right, moveSpeed));
            if (CheckHitKey(KEY_INPUT_D)) move = VAdd(move, VScale(right, -moveSpeed));

            // Прыжок
            static bool jumpPressed = false;
            if (CheckHitKey(KEY_INPUT_SPACE)) {
                if (!jumpPressed && onGround) {
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
            playerVel.y += gravity * gameSpeed;

            VECTOR newPos = VAdd(playerPos, VAdd(move, VGet(0, playerVel.y, 0)));

            // === КОЛЛИЗИИ ===
            Level* currentLevel = levelManager.GetCurrentLevel();
            if (currentLevel) {
                currentLevel->Update(effectiveDeltaTime);

                VECTOR platformVelocity = VGet(0, 0, 0);
                currentLevel->CheckCollision(playerPos, playerSize, newPos, playerVel, onGround, platformVelocity);

                newPos = VAdd(newPos, platformVelocity);
                playerPos = newPos;

                if (currentLevel->CheckDeadlyTrigger(playerPos, playerSize)) {
                    levelManager.RestartLevel();
                    playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                    playerVel = VGet(0, 0, 0);
                }

                VECTOR teleportTarget;
                if (teleportCooldown <= 0 && currentLevel->CheckTeleportTrigger(playerPos, playerSize, teleportTarget)) {
                    playerPos = teleportTarget;
                    playerVel = VGet(0, 0, 0);
                    teleportCooldown = 1.0f;
                }

                currentLevel->ActivateButton(playerPos, playerSize, activateKeyPressed);

                if (currentLevel->CheckWinTrigger(playerPos, playerSize)) {
                    levelManager.NextLevel();
                    playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                    playerVel = VGet(0, 0, 0);
                }
            }

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

            if (levelManager.GetCurrentLevel()) {
                levelManager.GetCurrentLevel()->Draw();
            }

            DrawCube3D(playerPos, VAdd(playerPos, playerSize),
                GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);

            levelManager.DrawLevelInfo();

            DrawFormatString(10, 140, GetColor(255, 255, 0), L"Player: X=%.1f Y=%.1f Z=%.1f",
                playerPos.x, playerPos.y, playerPos.z);

            unsigned int speedColor = GetColor(100, 255, 100);
            if (gameSpeed > 1.0f) speedColor = GetColor(255, 200, 100);
            if (gameSpeed < 1.0f) speedColor = GetColor(100, 200, 255);
            DrawFormatString(10, 160, speedColor, L"Game Speed: %.2fx ([ ] to change, 0 to reset)", gameSpeed);
        }

        ScreenFlip();
    }

END_GAME:
    DxLib_End();
    return 0;
}