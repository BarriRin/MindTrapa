#include "DxLib.h"
#include "Types.h"
#include "AudioManager.h"
#include "Camera.h"
#include "LevelManager.h"
#include "Menu.h"
#include "ModelManager.h"
#include "ParticleSystem.h"
#include "Player.h"
#include "ProfileManager.h"
#include "Renderer.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <string>

// ── Star thresholds: {3★ limit, 2★ limit} in seconds ─────────────────────────
const float STAR_THRESHOLDS[50][2] = {
    {10.0f, 15.0f},  // 1
    {10.0f, 15.0f},  // 2
    {10.0f, 15.0f},  // 3
    {10.0f, 15.0f},  // 4
    {10.0f, 15.0f},  // 5
    {10.0f, 15.0f},  // 6
    {10.0f, 15.0f},  // 7
    {10.0f, 15.0f},  // 8
    {15.0f, 22.5f},  // 9
    {20.0f, 30.0f},  // 10
    {15.0f, 22.5f},  // 11
    {15.0f, 22.5f},  // 12
    {20.0f, 30.0f},  // 13
    {15.0f, 22.5f},  // 14
    {15.0f, 22.5f},  // 15
    {15.0f, 22.5f},  // 16
    {25.0f, 37.5f},  // 17
    {20.0f, 30.0f},  // 18
    {25.0f, 37.5f},  // 19
    {30.0f, 45.0f},  // 20
    {10.0f, 15.0f},  // 21
    {10.0f, 15.0f},  // 22
    {15.0f, 22.5f},  // 23
    {10.0f, 15.0f},  // 24
    {10.0f, 15.0f},  // 25
    {10.0f, 15.0f},  // 26
    {15.0f, 22.5f},  // 27
    {15.0f, 22.5f},  // 28
    {15.0f, 22.5f},  // 29
    {25.0f, 37.5f},  // 30
    {15.0f, 22.5f},  // 31
    {20.0f, 30.0f},  // 32
    {15.0f, 22.5f},  // 33
    {15.0f, 22.5f},  // 34
    {15.0f, 22.5f},  // 35
    {15.0f, 22.5f},  // 36
    {15.0f, 22.5f},  // 37
    {15.0f, 22.5f},  // 38
    {20.0f, 30.0f},  // 39
    {25.0f, 37.5f},  // 40
    {15.0f, 22.5f},  // 41
    {15.0f, 22.5f},  // 42
    {15.0f, 22.5f},  // 43
    {15.0f, 22.5f},  // 44
    {30.0f, 45.0f},  // 45
    {15.0f, 22.5f},  // 46
    {15.0f, 22.5f},  // 47
    {20.0f, 30.0f},  // 48
    {20.0f, 30.0f},  // 49
    {25.0f, 37.5f},  // 50
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(FALSE);
    SetGraphMode(1920, 1080, 32);
    if (DxLib_Init() == -1) return -1;

    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    SetUseBackCulling(TRUE);
    SetUseLighting(FALSE);
    SetDrawScreen(DX_SCREEN_BACK);
    SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
    SetCameraNearFar(0.1f, 1000.0f);

    // ── ModelManager ──────────────────────────────────────────────────────────
    ModelManager& modelMgr = ModelManager::GetInstance();
    modelMgr.Initialize();

    modelMgr.LoadModel(ModelID::SPIKES,             "converted_x/Cube_Spikes.x",       1.0f);
    modelMgr.LoadModel(ModelID::FAKE_SPIKES,        "converted_x/Cube_Spikes.x",       1.0f);
    modelMgr.LoadModel(ModelID::RETRACTABLE_SPIKES, "converted_x/Cube_Spikes.x",       1.0f);
    modelMgr.LoadModel(ModelID::TRIGGER,            "converted_x/Goal_Flag.x",         1.0f);
    modelMgr.LoadModel(ModelID::BUTTON,             "converted_x/Cube_Exclamation.x",  1.0f);
    modelMgr.LoadModel(ModelID::TELEPORT,           "converted_x/Gem_Pink.x",          1.0f);
    modelMgr.LoadModel(ModelID::PLATFORM,           "converted_x/Cube_Grass_Single.x", 1.0f);
    modelMgr.LoadModel(ModelID::FAKE_PLATFORM,      "converted_x/Cube_Grass_Single.x", 1.0f);
    modelMgr.LoadModel(ModelID::CRUMBLING,          "converted_x/Cube_Crate.x",        1.0f);
    modelMgr.LoadModel(ModelID::MOVING,             "converted_x/Cube_Dirt_Single.x",  1.0f);
    modelMgr.LoadModel(ModelID::DISAPPEARING,       "converted_x/Cube_Bricks.x",       1.0f);

    // ── Биом и звёзды (объявляем до лямбды, т.к. она захватывает по ссылке) ──
    int              currentBiome = 0;
    std::vector<Star> stars;
    int skyboxTextures[5] = {-1,-1,-1,-1,-1};

    // ── Тема платформ по блокам ───────────────────────────────────────────────
    int currentThemeGroup = 0;
    auto applyBlockTheme = [&](int blockIdx) {
        int theme = (blockIdx <= 1) ? 0 : (blockIdx <= 3) ? 1 : 2;
        // Биом 1:1 к блоку (0-4)
        if (blockIdx != currentBiome) {
            currentBiome = blockIdx;
            stars = GenerateStars(1000, 400.0f, currentBiome);
        }
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
            modelMgr.LoadModel(ModelID::BOUNCE_PAD,    "converted_x/Bouncer.x",            1.0f);
        }
    };

    // ── Профили и меню ────────────────────────────────────────────────────────
    ProfileManager& profileMgr = ProfileManager::GetInstance();
    profileMgr.Initialize();

    LevelManager levelManager;
    Menu         menu(50);

    if (!profileMgr.HasActiveProfile())
        menu.OpenProfileSelect(false);
    else
        menu.SyncSettingsFromProfile();

    // ── Паки оформления (папка внутри media/: Defolt_Pack/Pack1/Pack2) ─────────
    auto packFolder = [](int pack) -> std::wstring {
        switch (pack) {
            case 1:  return L"Pack1";
            case 2:  return L"Pack2";
            default: return L"Defolt_Pack";
        }
    };

    // ── Аудио ─────────────────────────────────────────────────────────────────
    AudioManager audio;
    audio.LoadBgm(packFolder(profileMgr.GetThemePack()));
    audio.LoadSfx();

    // ── Видеофон меню ─────────────────────────────────────────────────────────
    int menuBgMovie = OpenMovieToGraph(L"media/menu_bg.mp4", TRUE);
    if (menuBgMovie != -1) PlayMovieToGraph(menuBgMovie, DX_PLAYTYPE_LOOP);

    // ── Скайбокс-текстуры (media/<Pack>/skybox/biomeN, -1 = файл не найден)
    // Пробуем JPG, потом PNG как fallback
    auto loadSkybox = [](const wchar_t* jpgPath, const wchar_t* pngPath) -> int {
        int h = LoadGraph(jpgPath);
        return (h != -1) ? h : LoadGraph(pngPath);
    };
    auto loadSkyboxPack = [&](int pack) {
        for (int i = 0; i < 5; i++) {
            if (skyboxTextures[i] != -1) { DeleteGraph(skyboxTextures[i]); skyboxTextures[i] = -1; }
        }
        std::wstring base = L"media/" + packFolder(pack) + L"/skybox/biome";
        for (int i = 0; i < 5; i++) {
            std::wstring path = base + std::to_wstring(i);
            skyboxTextures[i] = loadSkybox((path + L".jpg").c_str(), (path + L".png").c_str());
        }
    };
    loadSkyboxPack(profileMgr.GetThemePack());

    // ── Звёздный фон ─────────────────────────────────────────────────────────
    stars = GenerateStars(1000, 400.0f, currentBiome);

    // ── Игрок ────────────────────────────────────────────────────────────────
    Player player;
    player.LoadModel(L"models/converted_x/Character.mv1");

    // ── Камера ───────────────────────────────────────────────────────────────
    Camera camera;

    // ── Частицы ──────────────────────────────────────────────────────────────
    srand((unsigned int)GetNowCount());
    ParticleSystem particles;

    // ── Состояние игры ───────────────────────────────────────────────────────
    GameState gameState    = GameState::MAIN_MENU;
    float     darknessLevel = 0.0f;
    float     gameSpeed    = 1.0f;
    bool      debugMode    = false;

    // Телепорт антиспам
    float teleportCooldown = 0.0f;

    // Экран результата
    float dyingTimer        = 0.0f;
    bool  levelWasCompleted = false;

    // Праздник победы
    const float CELEBRATION_DURATION = 2.8f;
    float celebrationTimer  = 0.0f;
    VECTOR celebrationFlagPos = VGet(0,0,0);
    int   pendingCelebStars = 0;
    float pendingCelebTime  = 0.0f;
    bool  pendingCelebIsLast = false;
    int   celebPhase        = 0;
    bool  secondBurstFired  = false;

    // Таймер
    int   lastFrameTime  = GetNowCount();
    const float maxDeltaTime = 0.1f;

    // Переключение пака оформления (музыка+скайбоксы) — вызывается из меню Theme Packs.
    // Если игрок сменил пак прямо во время партии (пауза → настройки), тут же
    // перезапускает BGM текущего блока новым треком.
    auto applyThemePack = [&](int pack) {
        if (!profileMgr.IsThemePackUnlocked(pack)) return;
        profileMgr.SetThemePack(pack);
        loadSkyboxPack(pack);
        audio.LoadBgm(packFolder(pack));
        if (gameState == GameState::PLAYING || gameState == GameState::PAUSED) {
            int blockIdx = (levelManager.GetCurrentLevelId() - 1) / 10;
            audio.PlayBgm(blockIdx, menu.GetSettings().musicVolume);
        }
    };

    SetMouseDispFlag(TRUE);

    // ═════════════════════════════════════════════════════════════════════════
    // ОСНОВНОЙ ИГРОВОЙ ЦИКЛ
    // ═════════════════════════════════════════════════════════════════════════
    while (ProcessMessage() == 0) {
        ClearDrawScreen();

        int   now = GetNowCount();
        float realDt = (now - lastFrameTime) / 1000.0f;
        if (realDt > maxDeltaTime) realDt = maxDeltaTime;
        lastFrameTime = now;
        float dt = realDt * gameSpeed;

        // ── МЕНЮ ─────────────────────────────────────────────────────────────
        if (gameState == GameState::PROFILE_SELECT || gameState == GameState::PROFILE_CREATE ||
            gameState == GameState::MAIN_MENU      || gameState == GameState::LEVEL_SELECT_BLOCKS ||
            gameState == GameState::LEVEL_SELECT_LEVELS || gameState == GameState::SETTINGS ||
            gameState == GameState::MUSIC_SELECT)
        {
            SetMouseDispFlag(TRUE);

            if (menuBgMovie != -1) {
                DrawExtendGraph(0, 0, 1920, 1080, menuBgMovie, FALSE);
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
                DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                const Profile* p = profileMgr.GetCurrentProfile();
                // Аудио видео тише BGM-треков по записи — сдвигаем кривую так,
                // чтобы максимум громкости видео (10000) достигался уже на 75% ползунка
                int vol = p ? p->musicVolume * 10000 / 75 : 10000;
                if (vol > 10000) vol = 10000;
                SetMovieVolumeToGraph(vol, menuBgMovie);
            }

            menu.Draw();

            int blockToLoad = -1, levelToLoad = -1;
            ButtonAction action = menu.HandleInput(blockToLoad, levelToLoad);

            switch (action) {
            case ButtonAction::START_GAME:
                menu.ClearHistory();
                menu.SetState(GameState::PLAYING);
                applyBlockTheme(0);
                levelManager.LoadLevel(1);
                player.Reset(levelManager.GetCurrentLevel()->GetPlayerSpawn());
                darknessLevel = 0.0f;
                gameState = GameState::PLAYING;
                if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
                audio.PlayBgm(0, menu.GetSettings().musicVolume);
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

            case ButtonAction::SELECT_THEME_PACK_0:
                applyThemePack(0);
                menu.SetState(GameState::MUSIC_SELECT); // перерисовать список с новой пометкой "текущий"
                gameState = menu.GetState();
                break;

            case ButtonAction::SELECT_THEME_PACK_1:
                applyThemePack(1);
                menu.SetState(GameState::MUSIC_SELECT);
                gameState = menu.GetState();
                break;

            case ButtonAction::SELECT_THEME_PACK_2:
                applyThemePack(2);
                menu.SetState(GameState::MUSIC_SELECT);
                gameState = menu.GetState();
                break;

            case ButtonAction::EXIT_GAME:
                goto END_GAME;

            default:
                if (levelToLoad > 0) {
                    menu.ClearHistory();
                    menu.SetState(GameState::PLAYING);
                    applyBlockTheme((levelToLoad - 1) / 10);
                    levelManager.LoadLevel(levelToLoad);
                    player.Reset(levelManager.GetCurrentLevel()->GetPlayerSpawn());
                    darknessLevel = 0.0f;
                    gameState = GameState::PLAYING;
                    if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
                    audio.PlayBgm((levelToLoad - 1) / 10, menu.GetSettings().musicVolume);
                    SetMouseDispFlag(FALSE);
                } else {
                    gameState = menu.GetState();
                }
                break;
            }
        }

        // ── ПАУЗА ────────────────────────────────────────────────────────────
        else if (gameState == GameState::PAUSED) {
            SetMouseDispFlag(TRUE);

            Level* currentLevel = levelManager.GetCurrentLevel();
            if (currentLevel) {
                camera.Apply(player.GetPos());
                SetUseLighting(FALSE);
                SetUseBackCulling(FALSE);
                if (skyboxTextures[currentBiome] != -1) {
                    DrawSkyboxSphere(player.GetPos(), skyboxTextures[currentBiome]);
                } else {
                    DrawSpaceSkybox(player.GetPos(), currentBiome);
                    DrawBiomePlanet(player.GetPos(), currentBiome);
                }
                DrawStars(stars, player.GetPos());
                ApplyBiomeFog(currentBiome);
                currentLevel->Draw(debugMode);
                player.Draw();
                DisableFog();
            }

            menu.Draw();

            int blockToLoad = -1, levelToLoad = -1;
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
                player.Reset(levelManager.GetCurrentLevel()->GetPlayerSpawn());
                darknessLevel = 0.0f;
                gameState = GameState::PLAYING;
                if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
                audio.PlayBgm((levelManager.GetCurrentLevelId() - 1) / 10, menu.GetSettings().musicVolume);
                SetMouseDispFlag(FALSE);
                break;

            case ButtonAction::OPEN_SETTINGS:
                menu.PushState(GameState::SETTINGS);
                gameState = menu.GetState();
                break;

            case ButtonAction::BACK_TO_MENU:
                audio.StopBgm();
                menu.SetSelectedBlock(0);
                menu.ClearHistory();
                menu.SetState(GameState::MAIN_MENU);
                gameState = menu.GetState();
                if (menuBgMovie != -1) PlayMovieToGraph(menuBgMovie, DX_PLAYTYPE_LOOP);
                SetMouseDispFlag(TRUE);
                break;

            default:
                if (levelToLoad > 0) {
                    menu.ClearHistory();
                    menu.SetState(GameState::PLAYING);
                    applyBlockTheme((levelToLoad - 1) / 10);
                    levelManager.LoadLevel(levelToLoad);
                    player.Reset(levelManager.GetCurrentLevel()->GetPlayerSpawn());
                    darknessLevel = 0.0f;
                    gameState = GameState::PLAYING;
                    audio.PlayBgm((levelToLoad - 1) / 10, menu.GetSettings().musicVolume);
                    SetMouseDispFlag(FALSE);
                } else {
                    gameState = menu.GetState();
                }
                break;
            }
        }

        // ── GAMEPLAY ─────────────────────────────────────────────────────────
        else if (gameState == GameState::PLAYING) {
            SetMouseDispFlag(FALSE);

            // ESC → пауза
            static bool escPressed = false;
            static float escDelay  = 0.0f;
            if (escDelay > 0.0f) escDelay -= dt;
            if (CheckHitKey(KEY_INPUT_ESCAPE)) {
                if (!escPressed && escDelay <= 0.0f) {
                    menu.PushState(GameState::PAUSED);
                    menu.ResetInputFlags();
                    gameState = menu.GetState();
                    SetMouseDispFlag(TRUE);
                    escDelay = 0.3f;
                }
                escPressed = true;
            } else { escPressed = false; }

            // F3 — debug
            static bool f3Pressed = false;
            if (CheckHitKey(KEY_INPUT_F3)) { if (!f3Pressed) debugMode = !debugMode; f3Pressed = true; }
            else f3Pressed = false;

            // Управление скоростью игры ([ ] + 0)
            static bool plusP = false, minusP = false, bracketLP = false, bracketRP = false, zeroP = false;
            if (CheckHitKey(KEY_INPUT_ADD) || CheckHitKey(KEY_INPUT_SEMICOLON)) {
                if (!plusP) { gameSpeed += 0.25f; if (gameSpeed > 3.0f) gameSpeed = 3.0f; } plusP = true;
            } else plusP = false;
            if (CheckHitKey(KEY_INPUT_SUBTRACT) || CheckHitKey(KEY_INPUT_MINUS)) {
                if (!minusP) { gameSpeed -= 0.25f; if (gameSpeed < 0.25f) gameSpeed = 0.25f; } minusP = true;
            } else minusP = false;
            if (CheckHitKey(KEY_INPUT_LBRACKET)) {
                if (!bracketLP) { gameSpeed -= 0.25f; if (gameSpeed < 0.25f) gameSpeed = 0.25f; } bracketLP = true;
            } else bracketLP = false;
            if (CheckHitKey(KEY_INPUT_RBRACKET)) {
                if (!bracketRP) { gameSpeed += 0.25f; if (gameSpeed > 3.0f) gameSpeed = 3.0f; } bracketRP = true;
            } else bracketRP = false;
            if (CheckHitKey(KEY_INPUT_0) || CheckHitKey(KEY_INPUT_NUMPAD0)) {
                if (!zeroP) gameSpeed = 1.0f; zeroP = true;
            } else zeroP = false;

            levelManager.UpdateTimer(dt);
            audio.SetBgmVolume(menu.GetSettings().musicVolume);

            // Block 4 / Level 48: темнота
            int  curLvlId    = levelManager.GetCurrentLevelId();
            bool inBlock4    = (curLvlId >= 31 && curLvlId <= 40);
            bool inPulseLevel = (curLvlId == 48 || curLvlId == 50);

            Level* currentLevel = levelManager.GetCurrentLevel();
            if ((inBlock4 || inPulseLevel) && currentLevel) {
                bool lightFlash = inPulseLevel && currentLevel->IsAnyLightPulseZoneActive();
                if (lightFlash) {
                    darknessLevel -= dt * 3.0f;
                    if (darknessLevel < 0.0f) darknessLevel = 0.0f;
                } else {
                    bool moving = CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_S) ||
                                  CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_D) ||
                                  CheckHitKey(KEY_INPUT_SPACE) || !player.IsOnGround();
                    const float DS = inPulseLevel ? 2.0f : 0.8f;
                    const float LS = inPulseLevel ? 0.5f : 0.2f;
                    if (moving) {
                        darknessLevel += dt * DS;
                        if (darknessLevel > 1.0f) darknessLevel = 1.0f;
                    } else {
                        darknessLevel -= dt * LS;
                        if (darknessLevel < 0.0f) darknessLevel = 0.0f;
                    }
                }
            }

            // Телепорт кулдаун
            if (teleportCooldown > 0) teleportCooldown -= dt;

            // Обновление уровня (движущиеся платформы, выдвижные шипы, исчезающие блоки)
            if (currentLevel) currentLevel->Update(dt);

            // Камера
            camera.Update(player.GetPos(), (float)menu.GetSettings().mouseSensitivity, dt);

            // Обновление игрока (физика, ввод, анимация)
            bool playerDied = player.Update(dt, camera, currentLevel, audio, menu.GetSettings().soundVolume);

            if (playerDied) {
                levelManager.OnPlayerDeath();
                dyingTimer       = 1.0f;
                levelWasCompleted = false;
                gameState        = GameState::DYING;
            }

            // Телепорт
            if (!playerDied && teleportCooldown <= 0 && currentLevel) {
                VECTOR teleTarget;
                if (currentLevel->CheckTeleportTrigger(player.GetPos(), player.GetSize(), teleTarget)) {
                    player.SetPos(teleTarget);
                    player.SetVel(VGet(0,0,0));
                    teleportCooldown = 1.0f;
                }
            }

            // Победа
            if (gameState == GameState::PLAYING && currentLevel &&
                currentLevel->CheckWinTrigger(player.GetPos(), player.GetSize()))
            {
                audio.PlayWin(menu.GetSettings().soundVolume);
                levelManager.OnLevelComplete();
                float t      = levelManager.GetCurrentLevelTime();
                int   lvlIdx = levelManager.GetCurrentLevelId() - 1;
                int   stars  = (t <= STAR_THRESHOLDS[lvlIdx][0]) ? 3
                             : (t <= STAR_THRESHOLDS[lvlIdx][1]) ? 2 : 1;
                profileMgr.SetLevelResult(levelManager.GetCurrentLevelId(), stars, t);
                menu.UpdateUnlockState();
                levelWasCompleted = true;

                pendingCelebStars  = stars;
                pendingCelebTime   = t;
                pendingCelebIsLast = levelManager.IsLastLevel();

                celebrationFlagPos = player.GetPos();
                for (const auto& block : currentLevel->GetBlocks()) {
                    if (block.type == BlockType::TRIGGER) {
                        celebrationFlagPos = VAdd(block.pos,
                            VGet(block.size.x * 0.5f, block.size.y * 0.5f, block.size.z * 0.5f));
                        break;
                    }
                }

                celebrationTimer = CELEBRATION_DURATION;
                celebPhase       = 0;
                secondBurstFired = false;
                particles.Clear();
                particles.SpawnBurst(celebrationFlagPos, 80);

                player.StartCelebration();
                menu.SetState(GameState::LEVEL_CELEBRATING);
                gameState = GameState::LEVEL_CELEBRATING;
            }

            // ── ОТРИСОВКА СЦЕНЫ ──────────────────────────────────────────────
            SetUseLighting(FALSE);
            SetUseBackCulling(FALSE);

            VECTOR camPos = camera.GetPosition(player.GetPos());
            // Скайбокс (без тумана)
            if (skyboxTextures[currentBiome] != -1) {
                DrawSkyboxSphere(player.GetPos(), skyboxTextures[currentBiome]);
            } else {
                DrawSpaceSkybox(player.GetPos(), currentBiome);
                DrawBiomePlanet(player.GetPos(), currentBiome);
            }
            bool isDarkLevel = inBlock4 || inPulseLevel;

            // Обычные уровни: звёзды до геометрии — платформы их перекрывают
            if (!isDarkLevel) DrawStars(stars, player.GetPos());

            ApplyBiomeFog(currentBiome);
            if (currentLevel) currentLevel->Draw(debugMode);
            player.DrawShadow(currentLevel);
            player.Draw();
            DisableFog();

            // Тёмные уровни: сначала оверлей темноты, потом звёзды — ADD пробивает мрак
            if (isDarkLevel) {
                if (darknessLevel > 0.01f) {
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(darknessLevel * 240.0f));
                    DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                }
                DrawStars(stars, player.GetPos());
            }

            levelManager.DrawLevelInfo(debugMode);

            if (debugMode) {
                VECTOR pos = player.GetPos();
                DrawFormatString(10, 140, GetColor(255,255,0), L"Player: X=%.1f Y=%.1f Z=%.1f", pos.x, pos.y, pos.z);
                unsigned int sc = (gameSpeed > 1.0f) ? GetColor(255,200,100)
                                : (gameSpeed < 1.0f) ? GetColor(100,200,255)
                                : GetColor(100,255,100);
                DrawFormatString(10, 160, sc, L"Game Speed: %.2fx ([ ] to change, 0 to reset)", gameSpeed);
                int fps = (realDt > 0.0001f) ? (int)(1.0f / realDt) : 0;
                DrawFormatString(10, 180, GetColor(200,200,200), L"FPS: %d (deltaTime: %.4fs)", fps, realDt);
                DrawFormatString(10, 200, GetColor(150,150,150), L"[F3] Toggle Debug Info");
                int loaded = 0;
                for (int i = 0; i < (int)ModelID::COUNT; i++)
                    if (modelMgr.IsModelLoaded(static_cast<ModelID>(i))) loaded++;
                DrawFormatString(10, 220, GetColor(100,200,255), L"Models: %d/%d loaded", loaded, (int)ModelID::COUNT);
                if (inBlock4 || inPulseLevel)
                    DrawFormatString(10, 240, GetColor(150,100,255), L"DARKNESS | level: %.2f", darknessLevel);
            }
        }

        // ── УМИРАНИЕ ─────────────────────────────────────────────────────────
        else if (gameState == GameState::DYING) {
            SetMouseDispFlag(FALSE);

            player.UpdateAnimOnly(dt, false);

            dyingTimer -= dt;
            if (dyingTimer <= 0.0f) {
                menu.ShowResultScreen(false, 0, levelManager.GetCurrentLevelTime());
                gameState = GameState::LEVEL_RESULT;
                SetMouseDispFlag(TRUE);
            }

            VECTOR camPos = camera.GetPosition(player.GetPos());
            SetCameraPositionAndTarget_UpVecY(camPos, VAdd(player.GetPos(), VGet(0,1,0)));
            SetUseLighting(FALSE);
            SetUseBackCulling(FALSE);
            if (skyboxTextures[currentBiome] != -1) {
                DrawSkyboxSphere(player.GetPos(), skyboxTextures[currentBiome]);
            } else {
                DrawSpaceSkybox(player.GetPos(), currentBiome);
                DrawBiomePlanet(player.GetPos(), currentBiome);
            }
            DrawStars(stars, player.GetPos());
            ApplyBiomeFog(currentBiome);
            if (levelManager.GetCurrentLevel()) levelManager.GetCurrentLevel()->Draw(debugMode);
            player.Draw();
            DisableFog();
            levelManager.DrawLevelInfo(false);
        }

        // ── ПРАЗДНИК ПОБЕДЫ ───────────────────────────────────────────────────
        else if (gameState == GameState::LEVEL_CELEBRATING) {
            SetMouseDispFlag(FALSE);

            celebrationTimer -= dt;
            if (!secondBurstFired && celebrationTimer < CELEBRATION_DURATION - 1.2f) {
                particles.SpawnBurst(celebrationFlagPos, 50);
                secondBurstFired = true;
            }

            particles.Update(dt);

            // Анимация: Yes (однократно, ~1.2с) → Wave (зацикленно до конца праздника)
            if (celebPhase == 0) {
                player.UpdateAnimOnly(dt, false);
                if (celebrationTimer < CELEBRATION_DURATION - 1.2f) {
                    player.PlayWave();
                    celebPhase = 1;
                }
            } else {
                player.UpdateAnimOnly(dt, true);
            }

            if (celebrationTimer <= 0.0f) {
                particles.Clear();
                menu.ShowResultScreen(true, pendingCelebStars, pendingCelebTime, pendingCelebIsLast);
                gameState = GameState::LEVEL_RESULT;
                SetMouseDispFlag(TRUE);
            }

            VECTOR camPos = camera.GetPosition(player.GetPos());
            SetCameraPositionAndTarget_UpVecY(camPos, VAdd(player.GetPos(), VGet(0,1,0)));
            SetUseLighting(FALSE);
            SetUseBackCulling(FALSE);
            if (skyboxTextures[currentBiome] != -1) {
                DrawSkyboxSphere(player.GetPos(), skyboxTextures[currentBiome]);
            } else {
                DrawSpaceSkybox(player.GetPos(), currentBiome);
                DrawBiomePlanet(player.GetPos(), currentBiome);
            }
            DrawStars(stars, player.GetPos());
            ApplyBiomeFog(currentBiome);
            if (levelManager.GetCurrentLevel()) levelManager.GetCurrentLevel()->Draw(false);
            player.DrawFacingCamera(camPos);
            particles.Draw();
            DisableFog();
            levelManager.DrawLevelInfo(false);
        }

        // ── ЭКРАН РЕЗУЛЬТАТА ──────────────────────────────────────────────────
        else if (gameState == GameState::LEVEL_RESULT) {
            SetMouseDispFlag(TRUE);

            VECTOR camPos = camera.GetPosition(player.GetPos());
            SetCameraPositionAndTarget_UpVecY(camPos, VAdd(player.GetPos(), VGet(0,1,0)));
            SetUseLighting(FALSE);
            SetUseBackCulling(FALSE);
            if (skyboxTextures[currentBiome] != -1) {
                DrawSkyboxSphere(player.GetPos(), skyboxTextures[currentBiome]);
            } else {
                DrawSpaceSkybox(player.GetPos(), currentBiome);
                DrawBiomePlanet(player.GetPos(), currentBiome);
            }
            DrawStars(stars, player.GetPos());
            ApplyBiomeFog(currentBiome);
            if (levelManager.GetCurrentLevel()) levelManager.GetCurrentLevel()->Draw(false);
            player.Draw();
            DisableFog();

            menu.Draw();

            int dummy1 = -1, dummy2 = -1;
            ButtonAction action = menu.HandleInput(dummy1, dummy2);

            static bool rKeyPressed = false;
            if (CheckHitKey(KEY_INPUT_R)) {
                if (!rKeyPressed) action = ButtonAction::RESTART;
                rKeyPressed = true;
            } else rKeyPressed = false;

            static bool spaceKeyPressed = false;
            if (CheckHitKey(KEY_INPUT_SPACE)) {
                if (!spaceKeyPressed && levelWasCompleted && !levelManager.IsLastLevel())
                    action = ButtonAction::NEXT_LEVEL;
                spaceKeyPressed = true;
            } else spaceKeyPressed = false;

            if (action == ButtonAction::RESTART) {
                applyBlockTheme((levelManager.GetCurrentLevelId() - 1) / 10);
                levelManager.ReloadCurrentLevel();
                player.Reset(levelManager.GetCurrentLevel()->GetPlayerSpawn());
                darknessLevel = 0.0f;
                menu.SetState(GameState::PLAYING);
                gameState = GameState::PLAYING;
                if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
                audio.PlayBgm((levelManager.GetCurrentLevelId() - 1) / 10, menu.GetSettings().musicVolume);
                SetMouseDispFlag(FALSE);
            } else if (action == ButtonAction::BACK_TO_MENU) {
                audio.StopBgm();
                menu.ClearHistory();
                menu.SetState(GameState::MAIN_MENU);
                gameState = GameState::MAIN_MENU;
                if (menuBgMovie != -1) PlayMovieToGraph(menuBgMovie, DX_PLAYTYPE_LOOP);
                SetMouseDispFlag(TRUE);
            } else if (action == ButtonAction::NEXT_LEVEL && levelWasCompleted) {
                int nextId = levelManager.GetCurrentLevelId() + 1;
                applyBlockTheme((nextId - 1) / 10);
                levelManager.NextLevel();
                player.Reset(levelManager.GetCurrentLevel()->GetPlayerSpawn());
                darknessLevel = 0.0f;
                menu.SetState(GameState::PLAYING);
                gameState = GameState::PLAYING;
                if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
                audio.PlayBgm((levelManager.GetCurrentLevelId() - 1) / 10, menu.GetSettings().musicVolume);
                SetMouseDispFlag(FALSE);
            }
        }

        ScreenFlip();
    }

END_GAME:
    audio.StopBgm();
    for (int i = 0; i < 5; i++) if (skyboxTextures[i] != -1) DeleteGraph(skyboxTextures[i]);
    modelMgr.Cleanup();
    DxLib_End();
    return 0;
}
