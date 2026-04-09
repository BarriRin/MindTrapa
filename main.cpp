#include "DxLib.h"
#include "LevelManager.h"
#include "Menu.h"
#include "ModelManager.h"
#include "Renderer.h"
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

    // Инициализация ModelManager
    ModelManager& modelMgr = ModelManager::GetInstance();
    modelMgr.Initialize();

    // Загрузка моделей (converted_x/ - конвертированные из Quaternius pack)
    modelMgr.LoadModel(ModelID::PLATFORM,           "converted_x/Cube_Grass_Single.x", 1.0f);
    modelMgr.LoadModel(ModelID::SPIKES,             "converted_x/Cube_Spikes.x",       1.0f);
    modelMgr.LoadModel(ModelID::FAKE_SPIKES,        "converted_x/Cube_Default.x",      1.0f);
    modelMgr.LoadModel(ModelID::CRUMBLING,          "converted_x/Cube_Crate.x",        1.0f);
    modelMgr.LoadModel(ModelID::MOVING,             "converted_x/Cube_Dirt_Single.x",  1.0f);
    modelMgr.LoadModel(ModelID::DISAPPEARING,       "converted_x/Cube_Bricks.x",       1.0f);
    modelMgr.LoadModel(ModelID::FAKE_PLATFORM,      "converted_x/Cube_Default.x",      1.0f);
    modelMgr.LoadModel(ModelID::TRIGGER,            "converted_x/Goal_Flag.x",         1.0f);
    modelMgr.LoadModel(ModelID::BUTTON,             "converted_x/Lever.x",             1.0f);
    modelMgr.LoadModel(ModelID::PENDULUM_BLADE,     "converted_x/Hazard_Saw.x",        1.0f);

    // TODO: Загрузка скайбоксов (когда будут готовы)
    // modelMgr.LoadSkybox(0, "block0_mars.mqo", 1000.0f);   // Block 1 (Mars)
    // modelMgr.LoadSkybox(1, "block1_moon.mqo", 1000.0f);   // Block 2 (Moon)
    // ...

    // Инициализация игры
    LevelManager levelManager;
    Menu menu(40); // 40 уровней - Block 1-3 (1-30) + Block 4 (31-40)

    // Генерируем звёзды для космического фона
    std::vector<Star> stars = GenerateStars(500, 400.0f);

    // ОТЛАДКА - проверяем что меню создалось правильно
    char debug[100];
    sprintf_s(debug, "Menu created with 40 levels, %d stars generated", (int)stars.size());
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

    // Debug mode (F3 toggle)
    bool debugMode = false;

    // Измерение реального времени для deltaTime
    int lastFrameTime = GetNowCount();  // Время в миллисекундах
    const float maxDeltaTime = 0.1f;    // Ограничение deltaTime (защита от лагов)

    // Антиспам для телепортов
    float teleportCooldown = 0.0f;

    // Block 4: уровень темноты (0.0 = полная видимость, 1.0 = полная тьма)
    float darknessLevel = 0.0f;

    // Скрываем курсор мыши только в игре
    SetMouseDispFlag(TRUE);

    // === ОСНОВНОЙ ИГРОВОЙ ЦИКЛ ===
    while (ProcessMessage() == 0) {
        ClearDrawScreen();

        // Вычисляем РЕАЛЬНЫЙ deltaTime на основе времени между кадрами
        int currentFrameTime = GetNowCount();
        float realDeltaTime = (currentFrameTime - lastFrameTime) / 1000.0f;  // Конвертируем мс в секунды
        lastFrameTime = currentFrameTime;

        // Ограничиваем deltaTime чтобы избежать больших скачков при лагах
        if (realDeltaTime > maxDeltaTime) {
            realDeltaTime = maxDeltaTime;
        }

        // Применяем контроль скорости игры (для тестирования)
        float effectiveDeltaTime = realDeltaTime * gameSpeed;

        // === ОБРАБОТКА В ЗАВИСИМОСТИ ОТ СОСТОЯНИЯ ===
        if (gameState == GameState::MAIN_MENU || gameState == GameState::LEVEL_SELECT_BLOCKS ||
            gameState == GameState::LEVEL_SELECT_LEVELS || gameState == GameState::SETTINGS ||
            gameState == GameState::MUSIC_SELECT) {
            // Показываем курсор в меню
            SetMouseDispFlag(TRUE);

            // Отрисовка меню (не меняем состояние, меню само управляет стеком)
            menu.Draw();

            // Обработка ввода в меню
            int blockToLoad = -1;
            int levelToLoad = -1;
            ButtonAction action = menu.HandleInput(blockToLoad, levelToLoad);

            switch (action) {
            case ButtonAction::START_GAME:
                // Быстрый старт - сразу загружаем Level 1
                menu.ClearHistory();  // Очищаем стек
                menu.SetState(GameState::PLAYING);  // Устанавливаем состояние меню в PLAYING
                levelManager.LoadLevel(1);
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
                darknessLevel = 0.0f;
                gameState = GameState::PLAYING;
                SetMouseDispFlag(FALSE);
                break;

            case ButtonAction::OPEN_LEVEL_SELECT:
                menu.PushState(GameState::LEVEL_SELECT_BLOCKS);
                gameState = menu.GetState();
                break;

            case ButtonAction::OPEN_SETTINGS:
                menu.PushState(GameState::SETTINGS);
                gameState = menu.GetState();
                break;

            case ButtonAction::OPEN_MUSIC_SELECT:
                menu.PushState(GameState::MUSIC_SELECT);
                gameState = menu.GetState();
                break;

            case ButtonAction::EXIT_GAME:
                goto END_GAME;

            default:
                // Загрузка конкретного уровня
                if (levelToLoad > 0) {
                    menu.ClearHistory();  // Очищаем стек
                    menu.SetState(GameState::PLAYING);  // Устанавливаем состояние меню в PLAYING
                    levelManager.LoadLevel(levelToLoad);
                    playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                    playerVel = VGet(0, 0, 0);
                    darknessLevel = 0.0f;
                    gameState = GameState::PLAYING;
                    SetMouseDispFlag(FALSE);
                }
                else {
                    // Если ничего не произошло в switch (например, ESC вызвал PopState),
                    // синхронизируем состояние из меню
                    gameState = menu.GetState();
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
                currentLevel->Draw(debugMode);
                DrawCube3D(playerPos, VAdd(playerPos, playerSize),
                    GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
            }

            // Рисуем меню паузы поверх
            menu.Draw();

            // Обработка ввода
            int blockToLoad = -1;
            int levelToLoad = -1;
            ButtonAction action = menu.HandleInput(blockToLoad, levelToLoad);

            switch (action) {
            case ButtonAction::CONTINUE:
                gameState = GameState::PLAYING;
                SetMouseDispFlag(FALSE);
                break;

            case ButtonAction::RESTART:
                levelManager.RestartLevel();
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
                darknessLevel = 0.0f;
                gameState = GameState::PLAYING;
                SetMouseDispFlag(FALSE);
                break;

            case ButtonAction::OPEN_SETTINGS:
                menu.PushState(GameState::SETTINGS);
                gameState = menu.GetState();
                break;

            case ButtonAction::BACK_TO_MENU:
                menu.SetSelectedBlock(0); // Сброс выбранного блока
                menu.ClearHistory(); // Очищаем стек при возврате в главное меню
                menu.SetState(GameState::MAIN_MENU);
                gameState = menu.GetState();
                SetMouseDispFlag(TRUE);
                break;

            default:
                // Резервный обработчик (в паузе levelToLoad всегда 0)
                if (levelToLoad > 0) {
                    menu.ClearHistory();  // Очищаем стек
                    menu.SetState(GameState::PLAYING);  // Устанавливаем состояние меню в PLAYING
                    levelManager.LoadLevel(levelToLoad);
                    playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                    playerVel = VGet(0, 0, 0);
                    darknessLevel = 0.0f;
                    gameState = GameState::PLAYING;
                    SetMouseDispFlag(FALSE);
                }
                else {
                    // Если ничего не произошло в switch (например, ESC вызвал PopState),
                    // синхронизируем состояние из меню
                    gameState = menu.GetState();
                }
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
                    menu.PushState(GameState::PAUSED);
                    menu.ResetInputFlags();  // Сбрасываем флаги ввода чтобы ESC не обработался сразу в меню
                    gameState = menu.GetState();
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

            // === F3 - DEBUG MODE TOGGLE ===
            static bool f3Pressed = false;
            if (CheckHitKey(KEY_INPUT_F3)) {
                if (!f3Pressed) {
                    debugMode = !debugMode;
                }
                f3Pressed = true;
            }
            else {
                f3Pressed = false;
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

            // === ОБНОВЛЕНИЕ ТАЙМЕРА УРОВНЯ ===
            levelManager.UpdateTimer(effectiveDeltaTime);

            // Block 4: определяем активен ли тёмный режим
            int currentLevelId = levelManager.GetCurrentLevelId();
            bool inBlock4 = (currentLevelId >= 31 && currentLevelId <= 40);

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

            // Скорость движения, нормализованная к 60 FPS (для совместимости со старыми значениями speed)
            float moveSpeed = speed * (effectiveDeltaTime * 60.0f);

            if (CheckHitKey(KEY_INPUT_W)) move = VAdd(move, VScale(forward, -moveSpeed));
            if (CheckHitKey(KEY_INPUT_S)) move = VAdd(move, VScale(forward, moveSpeed));
            if (CheckHitKey(KEY_INPUT_A)) move = VAdd(move, VScale(right, moveSpeed));
            if (CheckHitKey(KEY_INPUT_D)) move = VAdd(move, VScale(right, -moveSpeed));

            // Block 4: накопительная темнота — растёт при движении, спадает в покое
            if (inBlock4) {
                bool isMoving = CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_S) ||
                                CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_D) ||
                                CheckHitKey(KEY_INPUT_SPACE) || !onGround;
                const float DARKEN_SPEED  = 0.8f; // сек до полной тьмы при движении
                const float LIGHTEN_SPEED = 0.2f; // сек до полной видимости в покое
                if (isMoving) {
                    darknessLevel += effectiveDeltaTime * DARKEN_SPEED;
                    if (darknessLevel > 1.0f) darknessLevel = 1.0f;
                } else {
                    darknessLevel -= effectiveDeltaTime * LIGHTEN_SPEED;
                    if (darknessLevel < 0.0f) darknessLevel = 0.0f;
                }
            }

            // Прыжок
            static bool jumpPressed = false;
            if (CheckHitKey(KEY_INPUT_SPACE)) {
                if (!jumpPressed && onGround) {
                    // jumpPower нормализован к 60 FPS, не нужно умножать на deltaTime (это начальная скорость)
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
                    darknessLevel = 0.0f;
                }
                restartPressed = true;
            }
            else {
                restartPressed = false;
            }

            // === ФИЗИКА ===
            // Проверяем гравитационные зоны (Block 3)
            Level* currentLevel = levelManager.GetCurrentLevel();
            float gravityMultiplier = 1.0f;
            if (currentLevel) {
                gravityMultiplier = currentLevel->CheckGravityZone(playerPos, playerSize);
            }

            // Применяем гравитацию с множителем (нормализуем к 60 FPS)
            playerVel.y += gravity * gravityMultiplier * (effectiveDeltaTime * 60.0f);

            // Применяем вертикальную скорость (с учётом deltaTime)
            VECTOR verticalMove = VGet(0, playerVel.y * (effectiveDeltaTime * 60.0f), 0);
            VECTOR newPos = VAdd(playerPos, VAdd(move, verticalMove));

            // === КОЛЛИЗИИ ===
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
                    darknessLevel = 0.0f;
                }

                VECTOR teleportTarget;
                if (teleportCooldown <= 0 && currentLevel->CheckTeleportTrigger(playerPos, playerSize, teleportTarget)) {
                    playerPos = teleportTarget;
                    playerVel = VGet(0, 0, 0);
                    teleportCooldown = 1.0f;
                }

                currentLevel->ActivateButton(playerPos, playerSize, activateKeyPressed);

                if (currentLevel->CheckWinTrigger(playerPos, playerSize)) {
                    levelManager.OnLevelComplete(); // Сохраняем время перед переходом
                    levelManager.NextLevel();
                    playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                    playerVel = VGet(0, 0, 0);
                    darknessLevel = 0.0f;
                }
            }

            if (playerPos.y < -10) {
                levelManager.RestartLevel();
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
                darknessLevel = 0.0f;
            }

            // === УСТАНОВКА КАМЕРЫ ===
            VECTOR cameraPos = VGet(
                playerPos.x + cameraDistance * sin(cameraAngleY) * cos(cameraAngleX),
                playerPos.y + cameraDistance * sin(cameraAngleX) + 2.0f,
                playerPos.z + cameraDistance * cos(cameraAngleY) * cos(cameraAngleX)
            );

            SetCameraPositionAndTarget_UpVecY(cameraPos, VAdd(playerPos, VGet(0, 1, 0)));

            // === BLOCK 4: DARKNESS SETUP ===
            // Темнота зависит только от движения игрока, без зональных фаз

            // === ОТРИСОВКА ===
            SetUseLighting(FALSE);
            SetUseBackCulling(FALSE);

            // Рисуем сцену всегда одинаково
            DrawSpaceSkybox(playerPos);
            DrawStars(stars, cameraPos);
            if (levelManager.GetCurrentLevel()) {
                levelManager.GetCurrentLevel()->Draw(debugMode);
            }

            // === ТЕНЬ ПОД ИГРОКОМ (для depth perception) ===
            // Находим платформу под игроком для проекции тени
            if (levelManager.GetCurrentLevel()) {
                Level* currentLevelForShadow = levelManager.GetCurrentLevel();
                float shadowY = -100.0f; // По умолчанию очень низко
                float closestDistance = 1000.0f;

                // Ищем ближайшую платформу снизу
                for (const auto& block : currentLevelForShadow->GetBlocks()) {
                    if (block.type == BlockType::PLATFORM ||
                        block.type == BlockType::TRIGGER ||
                        (block.type == BlockType::DISAPPEARING && block.isActive)) {

                        // Проверяем что платформа под игроком (по X и Z)
                        if (playerPos.x >= block.pos.x - 2.0f && playerPos.x <= block.pos.x + block.size.x + 2.0f &&
                            playerPos.z >= block.pos.z - 2.0f && playerPos.z <= block.pos.z + block.size.z + 2.0f) {

                            // Платформа снизу
                            if (block.pos.y + block.size.y < playerPos.y) {
                                float distance = playerPos.y - (block.pos.y + block.size.y);
                                if (distance < closestDistance) {
                                    closestDistance = distance;
                                    shadowY = block.pos.y + block.size.y + 0.01f; // Чуть выше платформы
                                }
                            }
                        }
                    }
                }

                // Рисуем тень если нашли платформу
                if (shadowY > -99.0f) {
                    // Размер тени зависит от высоты (выше = больше тень)
                    float shadowSize = 0.8f + (closestDistance * 0.05f);
                    if (shadowSize > 2.0f) shadowSize = 2.0f; // Макс размер

                    // Полупрозрачная чёрная тень
                    int alpha = (int)(100.0f - (closestDistance * 5.0f));
                    if (alpha < 20) alpha = 20;
                    if (alpha > 100) alpha = 100;

                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
                    DrawCube3D(
                        VGet(playerPos.x - shadowSize/2, shadowY, playerPos.z - shadowSize/2),
                        VGet(playerPos.x + shadowSize/2, shadowY + 0.05f, playerPos.z + shadowSize/2),
                        GetColor(0, 0, 0), GetColor(0, 0, 0), TRUE
                    );
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                }
            }

            // Рисуем игрока
            DrawCube3D(playerPos, VAdd(playerPos, playerSize),
                GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);

            // === BLOCK 4: DARKNESS ===
            if (inBlock4 && darknessLevel > 0.01f) {
                int alpha = (int)(darknessLevel * 240.0f);
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
                DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }

            levelManager.DrawLevelInfo(debugMode);

            // === DEBUG INFO (F3 to toggle) ===
            if (debugMode) {
                DrawFormatString(10, 140, GetColor(255, 255, 0), L"Player: X=%.1f Y=%.1f Z=%.1f",
                    playerPos.x, playerPos.y, playerPos.z);

                unsigned int speedColor = GetColor(100, 255, 100);
                if (gameSpeed > 1.0f) speedColor = GetColor(255, 200, 100);
                if (gameSpeed < 1.0f) speedColor = GetColor(100, 200, 255);
                DrawFormatString(10, 160, speedColor, L"Game Speed: %.2fx ([ ] to change, 0 to reset)", gameSpeed);

                // FPS счётчик для отладки
                int currentFPS = (realDeltaTime > 0.0001f) ? (int)(1.0f / realDeltaTime) : 0;
                DrawFormatString(10, 180, GetColor(200, 200, 200), L"FPS: %d (deltaTime: %.4fs)", currentFPS, realDeltaTime);

                DrawFormatString(10, 200, GetColor(150, 150, 150), L"[F3] Toggle Debug Info");

                // Статус загрузки моделей
                int loadedModels = 0;
                for (int i = 0; i < (int)ModelID::COUNT; i++) {
                    if (modelMgr.IsModelLoaded(static_cast<ModelID>(i))) loadedModels++;
                }
                DrawFormatString(10, 220, GetColor(100, 200, 255), L"Models: %d/%d loaded",
                    loadedModels, (int)ModelID::COUNT);

                if (inBlock4) {
                    DrawFormatString(10, 240, GetColor(150, 100, 255), L"BLOCK4 | darkness: %.2f",
                        darknessLevel);
                }
            }
        }

        ScreenFlip();
    }

END_GAME:
    // Очистка ModelManager
    modelMgr.Cleanup();

    DxLib_End();
    return 0;
}