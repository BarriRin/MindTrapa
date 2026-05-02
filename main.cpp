#include "DxLib.h"
#include "LevelManager.h"
#include "Menu.h"
#include "ModelManager.h"
#include "ProfileManager.h"
#include "Renderer.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <string>

struct Particle {
    VECTOR pos, vel;
    float  life, maxLife;
    unsigned int color;
};

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

    // Интерактивные модели — единый стиль для всех блоков (Quaternius)
    modelMgr.LoadModel(ModelID::SPIKES,             "converted_x/Cube_Spikes.x",       1.0f);
    modelMgr.LoadModel(ModelID::FAKE_SPIKES,        "converted_x/Cube_Spikes.x",       1.0f);
    modelMgr.LoadModel(ModelID::RETRACTABLE_SPIKES, "converted_x/Cube_Spikes.x",       1.0f);
    modelMgr.LoadModel(ModelID::TRIGGER,            "converted_x/Goal_Flag.x",         1.0f);
    modelMgr.LoadModel(ModelID::BUTTON,             "converted_x/Cube_Exclamation.x",  1.0f);
    modelMgr.LoadModel(ModelID::TELEPORT,           "converted_x/Gem_Pink.x",          1.0f);
    // PENDULUM_BLADE — примитив (прямоугольник + цепь + точка подвеса, не нуждается в модели)

    // Платформенные модели блока 0 — стартовые (Quaternius, блоки 1-2)
    modelMgr.LoadModel(ModelID::PLATFORM,      "converted_x/Cube_Grass_Single.x", 1.0f);
    modelMgr.LoadModel(ModelID::FAKE_PLATFORM, "converted_x/Cube_Grass_Single.x", 1.0f);
    modelMgr.LoadModel(ModelID::CRUMBLING,     "converted_x/Cube_Crate.x",        1.0f);
    modelMgr.LoadModel(ModelID::MOVING,        "converted_x/Cube_Dirt_Single.x",  1.0f);
    modelMgr.LoadModel(ModelID::DISAPPEARING,  "converted_x/Cube_Bricks.x",       1.0f);

    // TODO: Загрузка скайбоксов (когда будут готовы)
    // modelMgr.LoadSkybox(0, "block0_mars.mqo", 1000.0f);   // Block 1 (Mars)
    // modelMgr.LoadSkybox(1, "block1_moon.mqo", 1000.0f);   // Block 2 (Moon)
    // ...

    // === BGM (фоновая музыка по блокам) ===
    // Пробует .ogg → .mp3 → .wav для каждого блока
    auto loadBgm = [](const wchar_t* name) -> int {
        const wchar_t* exts[] = { L".ogg", L".mp3", L".wav" };
        for (auto ext : exts) {
            std::wstring path = std::wstring(L"media/music/") + name + ext;
            int h = LoadSoundMem(path.c_str());
            if (h != -1) return h;
        }
        return -1;
    };

    int bgmHandles[5] = {
        loadBgm(L"block1"),
        loadBgm(L"block2"),
        loadBgm(L"block3"),
        loadBgm(L"block4"),
        loadBgm(L"block5"),
    };

    int currentBgmIdx = -1;  // индекс текущего играющего трека (0-4)

    // Лямбда переключения трека
    auto playBgm = [&](int blockIdx, int volumePct) {
        if (blockIdx < 0 || blockIdx >= 5) return;
        if (bgmHandles[blockIdx] == -1) return;
        if (currentBgmIdx == blockIdx) {
            // Уже играет нужный трек — только обновить громкость
            ChangeVolumeSoundMem(volumePct * 255 / 100, bgmHandles[blockIdx]);
            return;
        }
        // Остановить предыдущий
        if (currentBgmIdx >= 0 && bgmHandles[currentBgmIdx] != -1)
            StopSoundMem(bgmHandles[currentBgmIdx]);
        // Запустить новый
        ChangeVolumeSoundMem(volumePct * 255 / 100, bgmHandles[blockIdx]);
        PlaySoundMem(bgmHandles[blockIdx], DX_PLAYTYPE_LOOP, TRUE);
        currentBgmIdx = blockIdx;
    };

    auto stopBgm = [&]() {
        if (currentBgmIdx >= 0 && bgmHandles[currentBgmIdx] != -1)
            StopSoundMem(bgmHandles[currentBgmIdx]);
        currentBgmIdx = -1;
    };

    // Смена темы платформ при переходе между группами блоков:
    //   blockIdx 0-1 → тема 0 (Quaternius, блоки 1-2)
    //   blockIdx 2-3 → тема 1 (Space Station, блоки 3-4)
    //   blockIdx 4   → тема 2 (KayKit Space Base, блок 5)
    int currentThemeGroup = 0;
    auto applyBlockTheme = [&](int blockIdx) {
        int theme = (blockIdx <= 1) ? 0 : (blockIdx <= 3) ? 1 : 2;
        if (theme == currentThemeGroup) return;
        currentThemeGroup = theme;

        modelMgr.UnloadModel(ModelID::PLATFORM);
        modelMgr.UnloadModel(ModelID::FAKE_PLATFORM);
        modelMgr.UnloadModel(ModelID::CRUMBLING);
        modelMgr.UnloadModel(ModelID::MOVING);
        modelMgr.UnloadModel(ModelID::DISAPPEARING);
        modelMgr.UnloadModel(ModelID::ICE_PLATFORM);
        modelMgr.UnloadModel(ModelID::BOUNCE_PAD);

        if (theme == 0) {
            modelMgr.LoadModel(ModelID::PLATFORM,      "converted_x/Cube_Grass_Single.x", 1.0f);
            modelMgr.LoadModel(ModelID::FAKE_PLATFORM, "converted_x/Cube_Grass_Single.x", 1.0f);
            modelMgr.LoadModel(ModelID::CRUMBLING,     "converted_x/Cube_Crate.x",        1.0f);
            modelMgr.LoadModel(ModelID::MOVING,        "converted_x/Cube_Dirt_Single.x",  1.0f);
            modelMgr.LoadModel(ModelID::DISAPPEARING,  "converted_x/Cube_Bricks.x",       1.0f);
        } else if (theme == 1) {
            modelMgr.LoadModel(ModelID::PLATFORM,      "space_station/floor-panel.x",        1.0f);
            modelMgr.LoadModel(ModelID::FAKE_PLATFORM, "space_station/floor-panel.x",        1.0f);
            modelMgr.LoadModel(ModelID::CRUMBLING,     "space_station/container.x",          1.0f);
            modelMgr.LoadModel(ModelID::MOVING,        "space_station/container-flat.x",     1.0f);
            modelMgr.LoadModel(ModelID::DISAPPEARING,  "space_station/door-single-closed.x", 1.0f);
            modelMgr.LoadModel(ModelID::BOUNCE_PAD,    "converted_x/Bouncer.x",             1.0f);
        } else {
            modelMgr.LoadModel(ModelID::PLATFORM,      "space_station/floor-panel.x",      1.0f);
            modelMgr.LoadModel(ModelID::FAKE_PLATFORM, "space_station/floor-panel.x",      1.0f);
            modelMgr.LoadModel(ModelID::CRUMBLING,     "space_kaykit/cargo_B_stacked.x",   1.0f);
            modelMgr.LoadModel(ModelID::MOVING,        "space_kaykit/cargo_A.x",           1.0f);
            modelMgr.LoadModel(ModelID::DISAPPEARING,  "space_kaykit/containers_A.x",      1.0f);
            // ICE_PLATFORM — примитив (голубой DrawCube3D), модель не нужна
            modelMgr.LoadModel(ModelID::BOUNCE_PAD,    "converted_x/Bouncer.x",            1.0f);
        }
    };

    // === МОДЕЛЬ ИГРОКА ===
    int playerModelHandle = MV1LoadModel(L"models/converted_x/Character.mv1");
    int playerAnimAttach  = -1;
    float playerAnimTime  = 0.0f;
    float playerFacingAngle = 0.0f;
    int playerCurrentAnim = -1;

    // Debug: записать все анимации в AnimList.txt
    if (playerModelHandle != -1) {
        int animCount = MV1GetAnimNum(playerModelHandle);
        FILE* f = nullptr;
        fopen_s(&f, "AnimList.txt", "w");
        if (f) {
            fprintf(f, "Character.mv1 — %d animations:\n", animCount);
            for (int i = 0; i < animCount; i++)
                fprintf(f, "  [%d] %ls\n", i, MV1GetAnimName(playerModelHandle, i));
            fclose(f);
        }
    }

    // Поиск анимации по подстроке имени
    auto findAnim = [&](const wchar_t* substr) -> int {
        if (playerModelHandle == -1) return 0;
        int n = MV1GetAnimNum(playerModelHandle);
        for (int i = 0; i < n; i++)
            if (wcsstr(MV1GetAnimName(playerModelHandle, i), substr)) return i;
        return 0;
    };
    int animIdle  = findAnim(L"Idle");
    int animRun   = findAnim(L"Run");
    int animJump  = findAnim(L"Jump");
    int animFall  = findAnim(L"Jump_Idle");  // [7] зависание/падение (Fall отсутствует в паке)
    int animDeath = findAnim(L"Death");
    if (animDeath == 0) animDeath = findAnim(L"Die");
    if (animDeath == 0) animDeath = findAnim(L"Defeat");
    int animWave  = findAnim(L"Wave");
    int animYes   = findAnim(L"Yes");

    // Видеофон главного меню
    int menuBgMovie = OpenMovieToGraph(L"media/menu_bg.mp4", TRUE);
    if (menuBgMovie != -1) {
        PlayMovieToGraph(menuBgMovie, DX_PLAYTYPE_LOOP);
    }

    // Профили
    ProfileManager& profileMgr = ProfileManager::GetInstance();
    profileMgr.Initialize();

    // Инициализация игры
    LevelManager levelManager;
    Menu menu(50); // 50 уровней - Block 1-5

    // Если нет активного профиля — показываем экран выбора
    if (!profileMgr.HasActiveProfile()) {
        menu.OpenProfileSelect(false);
    } else {
        menu.SyncSettingsFromProfile();
    }

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
    VECTOR playerHorizVel = VGet(0, 0, 0); // Block 5: персистентная горизонтальная скорость (для льда)
    float iceFriction = 1.0f;              // Block 5: текущий коэффициент трения льда (1.0 = не на льду)

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

    // Экран результата
    float dyingTimer        = 0.0f;
    bool  levelWasCompleted = false;

    // Система частиц (победный фейерверк)
    srand((unsigned int)GetNowCount());
    std::vector<Particle> particles;
    particles.reserve(200);

    const float CELEBRATION_DURATION = 2.8f;
    float        celebrationTimer    = 0.0f;
    VECTOR       celebrationFlagPos  = VGet(0.0f, 0.0f, 0.0f);
    int          pendingCelebStars   = 0;
    float        pendingCelebTime    = 0.0f;
    bool         pendingCelebIsLast  = false;
    int          celebPhase          = 0;    // 0=Yes, 1=Wave
    bool         secondBurstFired    = false;

    // Лямбда — выбросить burst частиц из точки origin
    auto spawnBurst = [&](VECTOR origin, int count) {
        static const unsigned int palette[] = {
            GetColor(255, 210,  50),  // gold
            GetColor(255, 255, 255),  // white
            GetColor( 80, 220, 255),  // cyan
            GetColor(255, 100, 200),  // pink
            GetColor(100, 255, 150),  // green
            GetColor(255, 160,  50),  // orange
        };
        for (int i = 0; i < count; i++) {
            Particle p;
            float az  = (rand() % 6284) * 0.001f;      // 0..2π azimuth
            float elev = ((rand() % 100) - 20) * 0.02f; // -0.4..1.6 elevation bias upward
            float spd  = 3.0f + (rand() % 100) * 0.06f; // 3..9 m/s
            p.vel  = VGet(cosf(elev) * cosf(az) * spd,
                          sinf(elev) * spd + 2.5f,
                          cosf(elev) * sinf(az) * spd);
            p.pos  = VAdd(origin, VGet((rand() % 100 - 50) * 0.01f,
                                       (rand() % 80)        * 0.01f,
                                       (rand() % 100 - 50) * 0.01f));
            p.maxLife = 1.4f + (rand() % 100) * 0.012f; // 1.4..2.6 s
            p.life    = p.maxLife;
            p.color   = palette[rand() % 6];
            particles.push_back(p);
        }
    };

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
        if (gameState == GameState::PROFILE_SELECT || gameState == GameState::PROFILE_CREATE ||
            gameState == GameState::MAIN_MENU || gameState == GameState::LEVEL_SELECT_BLOCKS ||
            gameState == GameState::LEVEL_SELECT_LEVELS || gameState == GameState::SETTINGS ||
            gameState == GameState::MUSIC_SELECT) {
            // Показываем курсор в меню
            SetMouseDispFlag(TRUE);

            // Видеофон
            if (menuBgMovie != -1) {
                DrawExtendGraph(0, 0, 1920, 1080, menuBgMovie, FALSE);
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
                DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                // Громкость из профиля
                const Profile* p = profileMgr.GetCurrentProfile();
                int vol = p ? p->musicVolume * 100 : 8000;
                SetMovieVolumeToGraph(vol, menuBgMovie);
            }

            // Отрисовка меню
            menu.Draw();

            // Обработка ввода в меню
            int blockToLoad = -1;
            int levelToLoad = -1;
            ButtonAction action = menu.HandleInput(blockToLoad, levelToLoad);

            switch (action) {
            case ButtonAction::START_GAME:
                // Быстрый старт - сразу загружаем Level 1
                menu.ClearHistory();
                menu.SetState(GameState::PLAYING);
                applyBlockTheme(0);
                levelManager.LoadLevel(1);
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
                darknessLevel = 0.0f;
                gameState = GameState::PLAYING;
                if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
                playBgm(0, menu.GetSettings().musicVolume);
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
                    menu.ClearHistory();
                    menu.SetState(GameState::PLAYING);
                    applyBlockTheme((levelToLoad - 1) / 10);
                    levelManager.LoadLevel(levelToLoad);
                    playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                    playerVel = VGet(0, 0, 0);
                    darknessLevel = 0.0f;
                    gameState = GameState::PLAYING;
                    if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
                    playBgm((levelToLoad - 1) / 10, menu.GetSettings().musicVolume);
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
                if (playerModelHandle != -1) {
                    VECTOR modelPos = VGet(playerPos.x + playerSize.x * 0.5f,
                                           playerPos.y,
                                           playerPos.z + playerSize.z * 0.5f);
                    float pScale = 0.006f;
                    MV1SetPosition(playerModelHandle, modelPos);
                    MV1SetScale(playerModelHandle, VGet(pScale, pScale, pScale));
                    MV1SetRotationXYZ(playerModelHandle, VGet(0.0f, playerFacingAngle + DX_PI_F, 0.0f));
                    MV1DrawModel(playerModelHandle);
                } else {
                    DrawCube3D(playerPos, VAdd(playerPos, playerSize),
                        GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
                }
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
                if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
                SetMouseDispFlag(FALSE);
                break;

            case ButtonAction::RESTART:
                applyBlockTheme((levelManager.GetCurrentLevelId() - 1) / 10);
                levelManager.RestartLevel();
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
                darknessLevel = 0.0f;
                gameState = GameState::PLAYING;
                if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
                playBgm((levelManager.GetCurrentLevelId() - 1) / 10, menu.GetSettings().musicVolume);
                SetMouseDispFlag(FALSE);
                break;

            case ButtonAction::OPEN_SETTINGS:
                menu.PushState(GameState::SETTINGS);
                gameState = menu.GetState();
                break;

            case ButtonAction::BACK_TO_MENU:
                stopBgm();
                menu.SetSelectedBlock(0);
                menu.ClearHistory();
                menu.SetState(GameState::MAIN_MENU);
                gameState = menu.GetState();
                if (menuBgMovie != -1) PlayMovieToGraph(menuBgMovie, DX_PLAYTYPE_LOOP);
                SetMouseDispFlag(TRUE);
                break;

            default:
                // Резервный обработчик (в паузе levelToLoad всегда 0)
                if (levelToLoad > 0) {
                    menu.ClearHistory();  // Очищаем стек
                    menu.SetState(GameState::PLAYING);  // Устанавливаем состояние меню в PLAYING
                    applyBlockTheme((levelToLoad - 1) / 10);
                    levelManager.LoadLevel(levelToLoad);
                    playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                    playerVel = VGet(0, 0, 0);
                    darknessLevel = 0.0f;
                    gameState = GameState::PLAYING;
                    playBgm((levelToLoad - 1) / 10, menu.GetSettings().musicVolume);
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

            // Синхронизация громкости BGM с настройками профиля
            if (currentBgmIdx >= 0 && bgmHandles[currentBgmIdx] != -1)
                ChangeVolumeSoundMem(menu.GetSettings().musicVolume * 255 / 100, bgmHandles[currentBgmIdx]);

            // Block 4: определяем активен ли тёмный режим
            int currentLevelId = levelManager.GetCurrentLevelId();
            bool inBlock4      = (currentLevelId >= 31 && currentLevelId <= 40);
            bool inPulseLevel  = (currentLevelId == 48 || currentLevelId == 50); // уровни 48+50: темнота со вспышками

            // === УПРАВЛЕНИЕ КАМЕРОЙ МЫШЬЮ ===
            int mouseX, mouseY;
            GetMousePoint(&mouseX, &mouseY);

            int centerX = 1920 / 2;
            int centerY = 1080 / 2;

            float mouseSens = 0.1f + menu.GetSettings().mouseSensitivity * 0.018f;
            cameraAngleY += (mouseX - centerX) * 0.002f * mouseSens;
            cameraAngleX += (mouseY - centerY) * 0.002f * mouseSens;

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

            // Block 4 + уровень 48: накопительная темнота
            if (inBlock4 || inPulseLevel) {
                Level* lvl = levelManager.GetCurrentLevel();
                bool lightFlash = inPulseLevel && lvl && lvl->IsAnyLightPulseZoneActive();
                if (lightFlash) {
                    // Вспышка — темнота быстро спадает
                    darknessLevel -= effectiveDeltaTime * 3.0f;
                    if (darknessLevel < 0.0f) darknessLevel = 0.0f;
                } else {
                    bool isMoving = CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_S) ||
                                    CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_D) ||
                                    CheckHitKey(KEY_INPUT_SPACE) || !onGround;
                    const float DARKEN_SPEED  = inPulseLevel ? 2.0f : 0.8f;
                    const float LIGHTEN_SPEED = inPulseLevel ? 0.5f : 0.2f;
                    if (isMoving) {
                        darknessLevel += effectiveDeltaTime * DARKEN_SPEED;
                        if (darknessLevel > 1.0f) darknessLevel = 1.0f;
                    } else {
                        darknessLevel -= effectiveDeltaTime * LIGHTEN_SPEED;
                        if (darknessLevel < 0.0f) darknessLevel = 0.0f;
                    }
                }
            }

            // Block 5: заранее проверяем лёд (нужно до прыжка)
            {
                Level* lvl = levelManager.GetCurrentLevel();
                iceFriction = lvl ? lvl->CheckIcePlatform(playerPos, playerSize) : 1.0f;
            }

            // Прыжок — на льду нельзя прыгать (только батут может выбросить)
            static bool jumpPressed = false;
            if (CheckHitKey(KEY_INPUT_SPACE)) {
                if (!jumpPressed && onGround && iceFriction >= 1.0f) {
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

            // === ФИЗИКА ===
            // Проверяем гравитационные зоны (Block 3)
            Level* currentLevel = levelManager.GetCurrentLevel();
            float gravityMultiplier = 1.0f;
            if (currentLevel) {
                gravityMultiplier = currentLevel->CheckGravityZone(playerPos, playerSize);
            }

            // Block 5: проверяем лёд и применяем инерцию
            // iceFriction уже вычислен выше (до прыжка)
            if (iceFriction < 1.0f) {
                // На льду: инерция при смене направления + скольжение без инпута
                bool hasInput = (move.x != 0.0f || move.z != 0.0f);
                if (hasInput) {
                    // Медленно меняем скорость — blend 0.04: разворот занимает ~35 кадров (~0.6 сек)
                    const float blend = 0.04f;
                    playerHorizVel.x = playerHorizVel.x * (1.f - blend) + move.x * blend;
                    playerHorizVel.z = playerHorizVel.z * (1.f - blend) + move.z * blend;
                } else {
                    // Без инпута: скользим, медленно тормозим
                    float decay = powf(iceFriction, effectiveDeltaTime * 60.0f);
                    playerHorizVel.x *= decay;
                    playerHorizVel.z *= decay;
                }
            } else {
                // Не на льду (обычная платформа или воздух) — мгновенное управление как раньше
                playerHorizVel = move;
            }

            // Применяем гравитацию с множителем (нормализуем к 60 FPS)
            playerVel.y += gravity * gravityMultiplier * (effectiveDeltaTime * 60.0f);

            // Применяем вертикальную скорость (с учётом deltaTime)
            VECTOR verticalMove = VGet(0, playerVel.y * (effectiveDeltaTime * 60.0f), 0);
            VECTOR newPos = VAdd(playerPos, VAdd(playerHorizVel, verticalMove));

            // === КОЛЛИЗИИ ===
            if (currentLevel) {
                currentLevel->Update(effectiveDeltaTime);

                VECTOR platformVelocity = VGet(0, 0, 0);
                currentLevel->CheckCollision(playerPos, playerSize, newPos, playerVel, onGround, platformVelocity);

                newPos = VAdd(newPos, platformVelocity);
                playerPos = newPos;

                if (currentLevel->CheckDeadlyTrigger(playerPos, playerSize)) {
                    levelManager.OnPlayerDeath();
                    dyingTimer = 1.0f;
                    levelWasCompleted = false;
                    playerVel = VGet(0, 0, 0);
                    playerHorizVel = VGet(0, 0, 0);
                    if (playerModelHandle != -1) {
                        if (playerAnimAttach != -1) MV1DetachAnim(playerModelHandle, playerAnimAttach);
                        playerAnimAttach  = MV1AttachAnim(playerModelHandle, animDeath, -1, FALSE);
                        playerAnimTime    = 0.0f;
                        playerCurrentAnim = animDeath;
                    }
                    gameState = GameState::DYING;
                }

                VECTOR teleportTarget;
                if (teleportCooldown <= 0 && currentLevel->CheckTeleportTrigger(playerPos, playerSize, teleportTarget)) {
                    playerPos = teleportTarget;
                    playerVel = VGet(0, 0, 0);
                    teleportCooldown = 1.0f;
                }

                currentLevel->ActivateButton(playerPos, playerSize, activateKeyPressed);

                if (gameState == GameState::PLAYING && currentLevel->CheckWinTrigger(playerPos, playerSize)) {
                    levelManager.OnLevelComplete();
                    float t         = levelManager.GetCurrentLevelTime();
                    int   starCount = (t <= 15.0f) ? 3 : (t <= 30.0f) ? 2 : 1;
                    profileMgr.SetLevelResult(levelManager.GetCurrentLevelId(), starCount, t);
                    menu.UpdateUnlockState();
                    levelWasCompleted = true;

                    // Сохраняем данные для result screen
                    pendingCelebStars  = starCount;
                    pendingCelebTime   = t;
                    pendingCelebIsLast = levelManager.IsLastLevel();

                    // Позиция флага в мировых координатах
                    celebrationFlagPos = playerPos;
                    for (const auto& block : currentLevel->GetBlocks()) {
                        if (block.type == BlockType::TRIGGER) {
                            celebrationFlagPos = VAdd(block.pos,
                                VGet(block.size.x * 0.5f, block.size.y * 0.5f, block.size.z * 0.5f));
                            break;
                        }
                    }

                    // Запускаем фейерверк
                    celebrationTimer   = CELEBRATION_DURATION;
                    celebPhase         = 0;
                    secondBurstFired   = false;
                    particles.clear();
                    spawnBurst(celebrationFlagPos, 80);

                    // Victory анимация: Yes → Wave
                    if (playerModelHandle != -1) {
                        if (playerAnimAttach != -1) MV1DetachAnim(playerModelHandle, playerAnimAttach);
                        playerAnimAttach  = MV1AttachAnim(playerModelHandle, animYes, -1, FALSE);
                        playerAnimTime    = 0.0f;
                        playerCurrentAnim = animYes;
                    }

                    menu.SetState(GameState::LEVEL_CELEBRATING);
                    gameState = GameState::LEVEL_CELEBRATING;
                    SetMouseDispFlag(FALSE);
                }
            }

            if (playerPos.y < -10) {
                levelManager.OnPlayerDeath();
                dyingTimer = 1.0f;
                levelWasCompleted = false;
                playerVel = VGet(0, 0, 0);
                playerHorizVel = VGet(0, 0, 0);
                if (playerModelHandle != -1) {
                    if (playerAnimAttach != -1) MV1DetachAnim(playerModelHandle, playerAnimAttach);
                    playerAnimAttach  = MV1AttachAnim(playerModelHandle, animDeath, -1, FALSE);
                    playerAnimTime    = 0.0f;
                    playerCurrentAnim = animDeath;
                }
                gameState = GameState::DYING;
            }

            // === АНИМАЦИЯ ИГРОКА ===
            if (gameState == GameState::PLAYING && playerModelHandle != -1) {
                bool isMovingH = (fabsf(move.x) > 0.001f || fabsf(move.z) > 0.001f);

                // Поворачиваем персонажа по направлению движения
                if (isMovingH)
                    playerFacingAngle = atan2f(move.x, move.z);

                // Стейт-машин: idle / run / jump / fall
                int targetAnim;
                if (!onGround) {
                    targetAnim = (playerVel.y > 0.0f) ? animJump : animFall;
                } else if (isMovingH) {
                    targetAnim = animRun;
                } else {
                    targetAnim = animIdle;
                }

                // Переключаем анимацию при смене состояния
                if (targetAnim != playerCurrentAnim) {
                    if (playerAnimAttach != -1)
                        MV1DetachAnim(playerModelHandle, playerAnimAttach);
                    playerAnimAttach = MV1AttachAnim(playerModelHandle, targetAnim, -1, FALSE);
                    playerAnimTime = 0.0f;
                    playerCurrentAnim = targetAnim;
                }

                // Продвигаем время (24 fps — из вьюера)
                if (playerAnimAttach != -1) {
                    float totalTime = MV1GetAttachAnimTotalTime(playerModelHandle, playerAnimAttach);
                    if (totalTime > 0.0f) {
                        playerAnimTime += effectiveDeltaTime * 24.0f;
                        if (playerAnimTime >= totalTime)
                            playerAnimTime = fmodf(playerAnimTime, totalTime);
                        MV1SetAttachAnimTime(playerModelHandle, playerAnimAttach, playerAnimTime);
                    }
                }
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
            if (playerModelHandle != -1) {
                // Центр AABB игрока по XZ, ноги на playerPos.y
                VECTOR modelPos = VGet(playerPos.x + playerSize.x * 0.5f,
                                       playerPos.y,
                                       playerPos.z + playerSize.z * 0.5f);
                float pScale = 0.006f;
                MV1SetPosition(playerModelHandle, modelPos);
                MV1SetScale(playerModelHandle, VGet(pScale, pScale, pScale));
                // +PI чтобы персонаж смотрел вперёд (Quaternius модели смотрят -Z)
                MV1SetRotationXYZ(playerModelHandle, VGet(0.0f, playerFacingAngle + DX_PI_F, 0.0f));
                MV1DrawModel(playerModelHandle);
            } else {
                DrawCube3D(playerPos, VAdd(playerPos, playerSize),
                    GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
            }

            // === BLOCK 4 / LEVEL 48: DARKNESS ===
            if ((inBlock4 || inPulseLevel) && darknessLevel > 0.01f) {
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

                if (inBlock4 || inPulseLevel) {
                    DrawFormatString(10, 240, GetColor(150, 100, 255), L"DARKNESS | level: %.2f",
                        darknessLevel);
                }
            }
        }
        else if (gameState == GameState::DYING) {
            SetMouseDispFlag(FALSE);

            // Обновляем анимацию смерти (один раз, без зацикливания)
            if (playerModelHandle != -1 && playerAnimAttach != -1) {
                float totalTime = MV1GetAttachAnimTotalTime(playerModelHandle, playerAnimAttach);
                if (totalTime > 0.0f) {
                    playerAnimTime += effectiveDeltaTime * 24.0f;
                    if (playerAnimTime > totalTime) playerAnimTime = totalTime;
                    MV1SetAttachAnimTime(playerModelHandle, playerAnimAttach, playerAnimTime);
                }
            }

            dyingTimer -= effectiveDeltaTime;
            if (dyingTimer <= 0.0f) {
                menu.ShowResultScreen(false, 0, levelManager.GetCurrentLevelTime());
                gameState = GameState::LEVEL_RESULT;
                SetMouseDispFlag(TRUE);
            }

            // Рисуем сцену (игрок виден на месте смерти)
            VECTOR cameraPos = VGet(
                playerPos.x + cameraDistance * sin(cameraAngleY) * cos(cameraAngleX),
                playerPos.y + cameraDistance * sin(cameraAngleX) + 2.0f,
                playerPos.z + cameraDistance * cos(cameraAngleY) * cos(cameraAngleX)
            );
            SetCameraPositionAndTarget_UpVecY(cameraPos, VAdd(playerPos, VGet(0, 1, 0)));
            SetUseLighting(FALSE);
            SetUseBackCulling(FALSE);
            DrawSpaceSkybox(playerPos);
            DrawStars(stars, cameraPos);
            if (levelManager.GetCurrentLevel()) levelManager.GetCurrentLevel()->Draw(debugMode);
            if (playerModelHandle != -1) {
                VECTOR modelPos = VGet(playerPos.x + playerSize.x * 0.5f,
                                       playerPos.y,
                                       playerPos.z + playerSize.z * 0.5f);
                float pScale = 0.006f;
                MV1SetPosition(playerModelHandle, modelPos);
                MV1SetScale(playerModelHandle, VGet(pScale, pScale, pScale));
                MV1SetRotationXYZ(playerModelHandle, VGet(0.0f, playerFacingAngle + DX_PI_F, 0.0f));
                MV1DrawModel(playerModelHandle);
            }
            levelManager.DrawLevelInfo(false);
        }
        else if (gameState == GameState::LEVEL_CELEBRATING) {
            SetMouseDispFlag(FALSE);

            // Таймер + второй burst
            celebrationTimer -= effectiveDeltaTime;
            if (!secondBurstFired && celebrationTimer < CELEBRATION_DURATION - 1.2f) {
                spawnBurst(celebrationFlagPos, 50);
                secondBurstFired = true;
            }

            // Обновление частиц
            for (auto& p : particles) {
                p.vel.y -= 8.0f * effectiveDeltaTime;
                p.pos    = VAdd(p.pos, VScale(p.vel, effectiveDeltaTime));
                p.life  -= effectiveDeltaTime;
            }
            particles.erase(
                std::remove_if(particles.begin(), particles.end(),
                    [](const Particle& p) { return p.life <= 0.0f; }),
                particles.end());

            // Анимация: Yes → Wave
            if (playerModelHandle != -1 && playerAnimAttach != -1) {
                float totalTime = MV1GetAttachAnimTotalTime(playerModelHandle, playerAnimAttach);
                if (totalTime > 0.0f) {
                    playerAnimTime += effectiveDeltaTime * 24.0f;
                    if (celebPhase == 0) {
                        // Yes: одиночное воспроизведение
                        if (playerAnimTime >= totalTime) {
                            MV1DetachAnim(playerModelHandle, playerAnimAttach);
                            playerAnimAttach  = MV1AttachAnim(playerModelHandle, animWave, -1, FALSE);
                            playerAnimTime    = 0.0f;
                            playerCurrentAnim = animWave;
                            celebPhase        = 1;
                        } else {
                            MV1SetAttachAnimTime(playerModelHandle, playerAnimAttach, playerAnimTime);
                        }
                    } else {
                        // Wave: loop
                        playerAnimTime = fmodf(playerAnimTime, totalTime);
                        MV1SetAttachAnimTime(playerModelHandle, playerAnimAttach, playerAnimTime);
                    }
                }
            }

            // Переход на экран результата
            if (celebrationTimer <= 0.0f) {
                particles.clear();
                menu.ShowResultScreen(true, pendingCelebStars, pendingCelebTime, pendingCelebIsLast);
                gameState = GameState::LEVEL_RESULT;
                SetMouseDispFlag(TRUE);
            }

            // Рендер сцены
            VECTOR celebCamPos = VGet(
                playerPos.x + cameraDistance * sinf(cameraAngleY) * cosf(cameraAngleX),
                playerPos.y + cameraDistance * sinf(cameraAngleX) + 2.0f,
                playerPos.z + cameraDistance * cosf(cameraAngleY) * cosf(cameraAngleX));
            SetCameraPositionAndTarget_UpVecY(celebCamPos, VAdd(playerPos, VGet(0, 1, 0)));
            SetUseLighting(FALSE);
            SetUseBackCulling(FALSE);
            DrawSpaceSkybox(playerPos);
            DrawStars(stars, celebCamPos);
            if (levelManager.GetCurrentLevel()) levelManager.GetCurrentLevel()->Draw(false);

            // Игрок — разворачиваем лицом к камере
            if (playerModelHandle != -1) {
                VECTOR modelPos = VGet(playerPos.x + playerSize.x * 0.5f,
                                       playerPos.y,
                                       playerPos.z + playerSize.z * 0.5f);
                float faceCamAngle = atan2f(celebCamPos.x - modelPos.x,
                                            celebCamPos.z - modelPos.z);
                MV1SetPosition(playerModelHandle, modelPos);
                MV1SetScale(playerModelHandle, VGet(0.006f, 0.006f, 0.006f));
                MV1SetRotationXYZ(playerModelHandle, VGet(0.0f, faceCamAngle + DX_PI_F, 0.0f));
                MV1DrawModel(playerModelHandle);
            }

            // Частицы: screen-space через ConvWorldPosToScreenPos
            for (const auto& p : particles) {
                VECTOR sp = ConvWorldPosToScreenPos(p.pos);
                if (sp.z <= 0.0f || sp.z >= 1.0f) continue;
                float lifeRatio = p.life / p.maxLife;
                int   a = (int)(lifeRatio * 220.0f);
                if (a < 5) continue;
                float r = 7.0f / (sp.z + 0.05f);
                if (r <  2.0f) r =  2.0f;
                if (r > 14.0f) r = 14.0f;
                // Core
                SetDrawBlendMode(DX_BLENDMODE_ADD, a);
                DrawCircle((int)sp.x, (int)sp.y, (int)r, p.color, TRUE);
                // Glow halo
                SetDrawBlendMode(DX_BLENDMODE_ADD, a / 4);
                DrawCircle((int)sp.x, (int)sp.y, (int)(r * 2.8f), p.color, TRUE);
            }
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            levelManager.DrawLevelInfo(false);
        }
        else if (gameState == GameState::LEVEL_RESULT) {
            SetMouseDispFlag(TRUE);

            // Рисуем сцену в фоне (игрок заморожен)
            VECTOR cameraPos = VGet(
                playerPos.x + cameraDistance * sin(cameraAngleY) * cos(cameraAngleX),
                playerPos.y + cameraDistance * sin(cameraAngleX) + 2.0f,
                playerPos.z + cameraDistance * cos(cameraAngleY) * cos(cameraAngleX)
            );
            SetCameraPositionAndTarget_UpVecY(cameraPos, VAdd(playerPos, VGet(0, 1, 0)));
            SetUseLighting(FALSE);
            SetUseBackCulling(FALSE);
            DrawSpaceSkybox(playerPos);
            DrawStars(stars, cameraPos);
            if (levelManager.GetCurrentLevel()) levelManager.GetCurrentLevel()->Draw(false);
            if (playerModelHandle != -1) {
                VECTOR modelPos = VGet(playerPos.x + playerSize.x * 0.5f,
                                       playerPos.y,
                                       playerPos.z + playerSize.z * 0.5f);
                float pScale = 0.006f;
                MV1SetPosition(playerModelHandle, modelPos);
                MV1SetScale(playerModelHandle, VGet(pScale, pScale, pScale));
                MV1SetRotationXYZ(playerModelHandle, VGet(0.0f, playerFacingAngle + DX_PI_F, 0.0f));
                MV1DrawModel(playerModelHandle);
            }

            // Оверлей + кнопки
            menu.Draw();

            // Ввод на экране результата
            int dummy1 = -1, dummy2 = -1;
            ButtonAction action = menu.HandleInput(dummy1, dummy2);

            // R → рестарт
            static bool rKeyPressed = false;
            if (CheckHitKey(KEY_INPUT_R)) {
                if (!rKeyPressed) action = ButtonAction::RESTART;
                rKeyPressed = true;
            } else {
                rKeyPressed = false;
            }

            // Space → следующий уровень (если прошли)
            static bool spaceKeyPressed = false;
            if (CheckHitKey(KEY_INPUT_SPACE)) {
                if (!spaceKeyPressed && levelWasCompleted && !levelManager.IsLastLevel()) {
                    action = ButtonAction::NEXT_LEVEL;
                }
                spaceKeyPressed = true;
            } else {
                spaceKeyPressed = false;
            }

            if (action == ButtonAction::RESTART) {
                applyBlockTheme((levelManager.GetCurrentLevelId() - 1) / 10);
                levelManager.ReloadCurrentLevel();
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
                playerHorizVel = VGet(0, 0, 0);
                darknessLevel = 0.0f;
                if (playerModelHandle != -1 && playerAnimAttach != -1) {
                    MV1DetachAnim(playerModelHandle, playerAnimAttach);
                    playerAnimAttach  = -1;
                    playerCurrentAnim = -1;
                }
                menu.SetState(GameState::PLAYING);
                gameState = GameState::PLAYING;
                if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
                playBgm((levelManager.GetCurrentLevelId() - 1) / 10, menu.GetSettings().musicVolume);
                SetMouseDispFlag(FALSE);
            } else if (action == ButtonAction::BACK_TO_MENU) {
                stopBgm();
                menu.ClearHistory();
                menu.SetState(GameState::MAIN_MENU);
                gameState = GameState::MAIN_MENU;
                if (menuBgMovie != -1) PlayMovieToGraph(menuBgMovie, DX_PLAYTYPE_LOOP);
                SetMouseDispFlag(TRUE);
            } else if (action == ButtonAction::NEXT_LEVEL && levelWasCompleted) {
                int nextLevelId = levelManager.GetCurrentLevelId() + 1;
                applyBlockTheme((nextLevelId - 1) / 10);
                levelManager.NextLevel();
                playerPos = levelManager.GetCurrentLevel()->GetPlayerSpawn();
                playerVel = VGet(0, 0, 0);
                playerHorizVel = VGet(0, 0, 0);
                darknessLevel = 0.0f;
                if (playerModelHandle != -1 && playerAnimAttach != -1) {
                    MV1DetachAnim(playerModelHandle, playerAnimAttach);
                    playerAnimAttach  = -1;
                    playerCurrentAnim = -1;
                }
                menu.SetState(GameState::PLAYING);
                gameState = GameState::PLAYING;
                if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
                playBgm((levelManager.GetCurrentLevelId() - 1) / 10, menu.GetSettings().musicVolume);
                SetMouseDispFlag(FALSE);
            }
        }

        ScreenFlip();
    }

END_GAME:
    // Очистка BGM
    stopBgm();
    for (int i = 0; i < 5; i++)
        if (bgmHandles[i] != -1) DeleteSoundMem(bgmHandles[i]);

    // Очистка игрока
    if (playerModelHandle != -1)
        MV1DeleteModel(playerModelHandle);

    // Очистка ModelManager
    modelMgr.Cleanup();

    DxLib_End();
    return 0;
}