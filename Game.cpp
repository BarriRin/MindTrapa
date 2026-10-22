#include "Game.h"
#include "LevelData.h"
#include <cstdlib>

namespace {
    struct ThemeModelEntry { ModelID id; const char* path; };

    // Platform theme group 0: Blocks 1-2 (grass/dirt)
    const ThemeModelEntry THEME_0_MODELS[] = {
        { ModelID::PLATFORM,      "converted_x/Cube_Grass_Single.x" },
        { ModelID::FAKE_PLATFORM, "converted_x/Cube_Grass_Single.x" },
        { ModelID::CRUMBLING,     "converted_x/Cube_Crate.x" },
        { ModelID::MOVING,        "converted_x/Cube_Dirt_Single.x" },
        { ModelID::DISAPPEARING,  "converted_x/Cube_Bricks.x" },
    };
    // Platform theme group 1: Block 3 (space station)
    const ThemeModelEntry THEME_1_MODELS[] = {
        { ModelID::PLATFORM,      "space_station/floor-panel.x" },
        { ModelID::FAKE_PLATFORM, "space_station/floor-panel.x" },
        { ModelID::CRUMBLING,     "space_station/container.x" },
        { ModelID::MOVING,        "space_station/container-flat.x" },
        { ModelID::DISAPPEARING,  "space_station/door-single-closed.x" },
        { ModelID::BOUNCE_PAD,    "converted_x/Bouncer.x" },
    };
    // Platform theme group 2: Blocks 4-5 (space station floor + KayKit cargo)
    const ThemeModelEntry THEME_2_MODELS[] = {
        { ModelID::PLATFORM,      "space_station/floor-panel.x" },
        { ModelID::FAKE_PLATFORM, "space_station/floor-panel.x" },
        { ModelID::CRUMBLING,     "space_kaykit/cargo_B_stacked.x" },
        { ModelID::MOVING,        "space_kaykit/cargo_A.x" },
        { ModelID::DISAPPEARING,  "space_kaykit/containers_A.x" },
        { ModelID::BOUNCE_PAD,    "converted_x/Bouncer.x" },
    };

    struct ThemeModelTable { const ThemeModelEntry* entries; int count; };
    const ThemeModelTable THEME_MODEL_TABLES[3] = {
        { THEME_0_MODELS, sizeof(THEME_0_MODELS) / sizeof(THEME_0_MODELS[0]) },
        { THEME_1_MODELS, sizeof(THEME_1_MODELS) / sizeof(THEME_1_MODELS[0]) },
        { THEME_2_MODELS, sizeof(THEME_2_MODELS) / sizeof(THEME_2_MODELS[0]) },
    };
}

Game::Game()
    : modelMgr(ModelManager::GetInstance())
    , profileMgr(ProfileManager::GetInstance())
{}

bool Game::Init() {
    ChangeWindowMode(FALSE);
    SetGraphMode(SCREEN_W, SCREEN_H, 32);
    if (DxLib_Init() == -1) return false;

    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    SetUseLighting(FALSE);
    SetDrawScreen(DX_SCREEN_BACK);
    SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
    SetCameraNearFar(0.1f, 1000.0f);

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

    // ── Profiles and menu ────────────────────────────────────────────────────
    profileMgr.Initialize();

    // LevelManager/Menu load content and fonts through DxLib in their
    // constructors, so they can only be created now that DxLib_Init() and the
    // base model loads above have already run
    levelManager = std::make_unique<LevelManager>();
    menu         = std::make_unique<Menu>(50);

    if (!profileMgr.HasActiveProfile())
        menu->OpenProfileSelect(false);
    else
        menu->SyncSettingsFromProfile();

    // ── Audio ─────────────────────────────────────────────────────────────────
    audio.LoadBgm(PackFolder(profileMgr.GetThemePack()));
    audio.LoadSfx();

    // ── Menu video background ────────────────────────────────────────────────
    menuBgMovie = OpenMovieToGraph(L"media/menu_bg.mp4", TRUE);
    if (menuBgMovie != -1) PlayMovieToGraph(menuBgMovie, DX_PLAYTYPE_LOOP);

    LoadSkyboxPack(profileMgr.GetThemePack());

    // ── Star field ────────────────────────────────────────────────────────────
    stars = GenerateStars(1000, 400.0f, currentBiome);

    // ── Player ────────────────────────────────────────────────────────────────
    player.LoadModel(L"models/converted_x/Character.mv1");

    srand((unsigned int)GetNowCount());

    lastFrameTime = GetNowCount();

    SetMouseDispFlag(TRUE);

    return true;
}

void Game::Run() {
    while (ProcessMessage() == 0) {
        ClearDrawScreen();
        input.Update();

        int   now = GetNowCount();
        float realDt = (now - lastFrameTime) / 1000.0f;
        if (realDt > maxDeltaTime) realDt = maxDeltaTime;
        lastFrameTime = now;
        float dt = realDt * gameSpeed;

        // BGM volume is applied live in every state (not just PLAYING), so the
        // music slider in settings is audible immediately regardless of where it was opened from
        audio.SetBgmVolume(menu->GetSettings().musicVolume);

        if (gameState == GameState::PROFILE_SELECT || gameState == GameState::PROFILE_CREATE ||
            gameState == GameState::MAIN_MENU      || gameState == GameState::LEVEL_SELECT_BLOCKS ||
            gameState == GameState::LEVEL_SELECT_LEVELS || gameState == GameState::SETTINGS ||
            gameState == GameState::MUSIC_SELECT)
        {
            UpdateMenu(realDt);
        } else if (gameState == GameState::PAUSED) {
            UpdatePaused();
        } else if (gameState == GameState::PLAYING) {
            UpdatePlaying(dt, realDt);
        } else if (gameState == GameState::DYING) {
            UpdateDying(dt);
        } else if (gameState == GameState::LEVEL_CELEBRATING) {
            UpdateCelebrating(dt);
        } else if (gameState == GameState::LEVEL_RESULT) {
            UpdateResult();
        }

        if (exitRequested) break;

        ScreenFlip();
    }
}

void Game::Shutdown() {
    // Explicitly release all DxLib resources BEFORE DxLib_End() — the
    // menu/audio/levelManager/player destructors would otherwise only run when
    // the Game object is destroyed in WinMain, i.e. after DxLib_End(), which
    // DxLib's documentation says isn't supported.
    for (int i = 0; i < 5; i++) if (skyboxTextures[i] != -1) DeleteGraph(skyboxTextures[i]);
    if (menuBgMovie != -1) DeleteGraph(menuBgMovie);
    audio.Shutdown();
    levelManager->Shutdown();
    player.ReleaseModel();
    menu->Shutdown();
    modelMgr.Cleanup();
    DxLib_End();
}

void Game::UpdateMenu(float realDt) {
    SetMouseDispFlag(TRUE);

    if (menuBgMovie != -1) {
        DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, menuBgMovie, FALSE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        const Profile* p = profileMgr.GetCurrentProfile();
        // The background video is quieter than the BGM tracks by nature —
        // remap the curve so the video reaches max volume (10000) at 75% of the slider
        int vol = p ? p->musicVolume * 10000 / 75 : 10000;
        if (vol > 10000) vol = 10000;
        SetMovieVolumeToGraph(vol, menuBgMovie);
    }

    menu->Draw();

    int dummyBlock = -1, levelToLoad = -1;
    ButtonAction action = menu->HandleInput(dummyBlock, levelToLoad);

    // Preview the SFX while dragging the sound slider — otherwise there's
    // nothing to listen to while adjusting effect volume from the settings screen
    {
        if (sfxPreviewCooldown > 0.0f) sfxPreviewCooldown -= realDt;

        bool inSettingsNow = (gameState == GameState::SETTINGS);
        if (inSettingsNow && !wasInSettings) {
            lastPreviewSoundVolume = menu->GetSettings().soundVolume; // sync without playing a sound
        }
        if (inSettingsNow) {
            int curSoundVolume = menu->GetSettings().soundVolume;
            if (curSoundVolume != lastPreviewSoundVolume && sfxPreviewCooldown <= 0.0f) {
                audio.PlayJump(curSoundVolume);
                sfxPreviewCooldown      = 0.15f;
                lastPreviewSoundVolume  = curSoundVolume;
            }
        }
        wasInSettings = inSettingsNow;
    }

    switch (action) {
    case ButtonAction::START_GAME:
        menu->ClearHistory();
        menu->SetState(GameState::PLAYING);
        ApplyBlockTheme(0);
        levelManager->LoadLevel(1);
        FinishLevelStart();
        break;

    case ButtonAction::OPEN_LEVEL_SELECT:
        menu->PushState(GameState::LEVEL_SELECT_BLOCKS);
        gameState = menu->GetState();
        break;

    case ButtonAction::OPEN_SETTINGS:
        menu->PushState(GameState::SETTINGS);
        gameState = menu->GetState();
        break;

    case ButtonAction::OPEN_MUSIC_SELECT:
        menu->PushState(GameState::MUSIC_SELECT);
        gameState = menu->GetState();
        break;

    case ButtonAction::SELECT_THEME_PACK_0:
        ApplyThemePack(0);
        menu->SetState(GameState::MUSIC_SELECT); // redraw the list with the new "current" marker
        gameState = menu->GetState();
        break;

    case ButtonAction::SELECT_THEME_PACK_1:
        ApplyThemePack(1);
        menu->SetState(GameState::MUSIC_SELECT);
        gameState = menu->GetState();
        break;

    case ButtonAction::SELECT_THEME_PACK_2:
        ApplyThemePack(2);
        menu->SetState(GameState::MUSIC_SELECT);
        gameState = menu->GetState();
        break;

    case ButtonAction::EXIT_GAME:
        exitRequested = true;
        break;

    default:
        if (levelToLoad > 0) {
            menu->ClearHistory();
            menu->SetState(GameState::PLAYING);
            ApplyBlockTheme((levelToLoad - 1) / 10);
            levelManager->LoadLevel(levelToLoad);
            FinishLevelStart();
        } else {
            gameState = menu->GetState();
        }
        break;
    }
}

void Game::UpdatePaused() {
    SetMouseDispFlag(TRUE);

    if (levelManager->GetCurrentLevel()) {
        DrawScene(debugMode, /*isDarkLevel=*/false, /*drawShadow=*/false, PlayerDrawMode::Normal, /*drawParticles=*/false);
    }

    menu->Draw();

    int dummyBlock = -1, levelToLoad = -1;
    ButtonAction action = menu->HandleInput(dummyBlock, levelToLoad);

    switch (action) {
    case ButtonAction::CONTINUE:
        gameState = GameState::PLAYING;
        if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
        SetMouseDispFlag(FALSE);
        break;

    case ButtonAction::RESTART:
        ApplyBlockTheme((levelManager->GetCurrentLevelId() - 1) / 10);
        levelManager->RestartLevel();
        FinishLevelStart();
        break;

    case ButtonAction::OPEN_SETTINGS:
        menu->PushState(GameState::SETTINGS);
        gameState = menu->GetState();
        break;

    case ButtonAction::BACK_TO_MENU:
        audio.StopBgm();
        menu->SetSelectedBlock(0);
        menu->ClearHistory();
        menu->SetState(GameState::MAIN_MENU);
        gameState = menu->GetState();
        if (menuBgMovie != -1) PlayMovieToGraph(menuBgMovie, DX_PLAYTYPE_LOOP);
        SetMouseDispFlag(TRUE);
        break;

    default:
        if (levelToLoad > 0) {
            menu->ClearHistory();
            menu->SetState(GameState::PLAYING);
            ApplyBlockTheme((levelToLoad - 1) / 10);
            levelManager->LoadLevel(levelToLoad);
            FinishLevelStart();
        } else {
            gameState = menu->GetState();
        }
        break;
    }
}

void Game::UpdatePlaying(float dt, float realDt) {
    SetMouseDispFlag(FALSE);

    // ESC -> pause
    static float escDelay = 0.0f;
    if (escDelay > 0.0f) escDelay -= dt;
    if (input.IsPressed(KEY_INPUT_ESCAPE) && escDelay <= 0.0f) {
        menu->PushState(GameState::PAUSED);
        menu->ResetInputFlags();
        gameState = menu->GetState();
        SetMouseDispFlag(TRUE);
        escDelay = 0.3f;
    }

    // F3 — debug
    if (input.IsPressed(KEY_INPUT_F3)) debugMode = !debugMode;

    // Game speed control ([ ] + 0)
    static bool plusP = false, minusP = false, bracketLP = false, bracketRP = false, zeroP = false;
    if (input.IsHeld(KEY_INPUT_ADD) || input.IsHeld(KEY_INPUT_SEMICOLON)) {
        if (!plusP) { gameSpeed += 0.25f; if (gameSpeed > 3.0f) gameSpeed = 3.0f; } plusP = true;
    } else plusP = false;
    if (input.IsHeld(KEY_INPUT_SUBTRACT) || input.IsHeld(KEY_INPUT_MINUS)) {
        if (!minusP) { gameSpeed -= 0.25f; if (gameSpeed < 0.25f) gameSpeed = 0.25f; } minusP = true;
    } else minusP = false;
    if (input.IsHeld(KEY_INPUT_LBRACKET)) {
        if (!bracketLP) { gameSpeed -= 0.25f; if (gameSpeed < 0.25f) gameSpeed = 0.25f; } bracketLP = true;
    } else bracketLP = false;
    if (input.IsHeld(KEY_INPUT_RBRACKET)) {
        if (!bracketRP) { gameSpeed += 0.25f; if (gameSpeed > 3.0f) gameSpeed = 3.0f; } bracketRP = true;
    } else bracketRP = false;
    if (input.IsHeld(KEY_INPUT_0) || input.IsHeld(KEY_INPUT_NUMPAD0)) {
        if (!zeroP) gameSpeed = 1.0f; zeroP = true;
    } else zeroP = false;

    levelManager->UpdateTimer(dt);

    // Block 4 / Level 48: darkness
    int  curLvlId     = levelManager->GetCurrentLevelId();
    bool inBlock4     = (curLvlId >= 31 && curLvlId <= 40);
    bool inPulseLevel = (curLvlId == 48 || curLvlId == 50);

    Level* currentLevel = levelManager->GetCurrentLevel();
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

    // Teleport cooldown
    if (teleportCooldown > 0) teleportCooldown -= dt;

    // Update the level (moving platforms, retractable spikes, disappearing blocks)
    if (currentLevel) currentLevel->Update(dt);

    // Camera angles — before player.Update(), so movement is relative to
    // this frame's look direction
    camera.UpdateAngles((float)menu->GetSettings().mouseSensitivity);

    // Update the player (physics, input, animation)
    bool playerDied = player.Update(dt, camera, currentLevel, audio, menu->GetSettings().soundVolume);

    if (playerDied) {
        levelManager->OnPlayerDeath();
        dyingTimer        = 1.0f;
        levelWasCompleted = false;
        gameState         = GameState::DYING;
    }

    // Teleport — before camera.Apply(), otherwise on the teleport frame the
    // camera would still look at the pre-jump position while the model is
    // already drawn at the post-jump one (the same one-frame desync we just
    // fixed for regular movement/falling)
    if (!playerDied && teleportCooldown <= 0 && currentLevel) {
        VECTOR teleTarget;
        if (currentLevel->CheckTeleportTrigger(player.GetPos(), player.GetSize(), teleTarget)) {
            player.SetPos(teleTarget);
            player.SetVel(VGet(0, 0, 0));
            teleportCooldown = 1.0f;
            audio.PlayTeleport(menu->GetSettings().soundVolume);
        }
    }

    // Camera position — after player.Update()/teleport, so it follows this
    // frame's player position rather than the previous one
    camera.Apply(player.GetPos());

    // Victory
    if (gameState == GameState::PLAYING && currentLevel &&
        currentLevel->CheckWinTrigger(player.GetPos(), player.GetSize()))
    {
        audio.PlayWin(menu->GetSettings().soundVolume);
        levelManager->OnLevelComplete();
        float t  = levelManager->GetCurrentLevelTime();
        const StarThresholds& th = GetStarThresholds(levelManager->GetCurrentLevelId());
        int   earnedStars = (t <= th.threeStarMaxTime) ? 3
                           : (t <= th.twoStarMaxTime) ? 2 : 1;
        profileMgr.SetLevelResult(levelManager->GetCurrentLevelId(), earnedStars, t);
        menu->UpdateUnlockState();
        levelWasCompleted = true;

        pendingCelebStars  = earnedStars;
        pendingCelebTime   = t;
        pendingCelebIsLast = levelManager->IsLastLevel();

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
        menu->SetState(GameState::LEVEL_CELEBRATING);
        gameState = GameState::LEVEL_CELEBRATING;
    }

    bool isDarkLevel = inBlock4 || inPulseLevel;
    DrawScene(debugMode, isDarkLevel, /*drawShadow=*/true, PlayerDrawMode::Normal, /*drawParticles=*/false);

    levelManager->DrawLevelInfo(debugMode);

    if (debugMode) {
        VECTOR pos = player.GetPos();
        DrawFormatString(10, 140, GetColor(255, 255, 0), L"Player: X=%.1f Y=%.1f Z=%.1f", pos.x, pos.y, pos.z);
        unsigned int sc = (gameSpeed > 1.0f) ? GetColor(255, 200, 100)
                         : (gameSpeed < 1.0f) ? GetColor(100, 200, 255)
                         : GetColor(100, 255, 100);
        DrawFormatString(10, 160, sc, L"Game Speed: %.2fx ([ ] to change, 0 to reset)", gameSpeed);
        int fps = (realDt > 0.0001f) ? (int)(1.0f / realDt) : 0;
        DrawFormatString(10, 180, GetColor(200, 200, 200), L"FPS: %d (deltaTime: %.4fs)", fps, realDt);
        DrawFormatString(10, 200, GetColor(150, 150, 150), L"[F3] Toggle Debug Info");
        int loaded = 0;
        for (int i = 0; i < (int)ModelID::COUNT; i++)
            if (modelMgr.IsModelLoaded(static_cast<ModelID>(i))) loaded++;
        DrawFormatString(10, 220, GetColor(100, 200, 255), L"Models: %d/%d loaded", loaded, (int)ModelID::COUNT);
        if (inBlock4 || inPulseLevel)
            DrawFormatString(10, 240, GetColor(150, 100, 255), L"DARKNESS | level: %.2f", darknessLevel);
    }
}

void Game::UpdateDying(float dt) {
    SetMouseDispFlag(FALSE);

    player.UpdateAnimOnly(dt, false);

    dyingTimer -= dt;
    if (dyingTimer <= 0.0f) {
        menu->ShowResultScreen(false, 0, levelManager->GetCurrentLevelTime());
        gameState = GameState::LEVEL_RESULT;
        SetMouseDispFlag(TRUE);
    }

    DrawScene(debugMode, /*isDarkLevel=*/false, /*drawShadow=*/false, PlayerDrawMode::Normal, /*drawParticles=*/false);
    levelManager->DrawLevelInfo(false);
}

void Game::UpdateCelebrating(float dt) {
    SetMouseDispFlag(FALSE);

    celebrationTimer -= dt;
    if (!secondBurstFired && celebrationTimer < CELEBRATION_DURATION - 1.2f) {
        particles.SpawnBurst(celebrationFlagPos, 50);
        secondBurstFired = true;
    }

    particles.Update(dt);

    // Animation: Yes (one-shot, ~1.2s) -> Wave (looping until the celebration ends)
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
        menu->ShowResultScreen(true, pendingCelebStars, pendingCelebTime, pendingCelebIsLast);
        gameState = GameState::LEVEL_RESULT;
        SetMouseDispFlag(TRUE);
    }

    DrawScene(false, /*isDarkLevel=*/false, /*drawShadow=*/false, PlayerDrawMode::FacingCamera, /*drawParticles=*/true);
    levelManager->DrawLevelInfo(false);
}

void Game::UpdateResult() {
    SetMouseDispFlag(TRUE);

    DrawScene(false, /*isDarkLevel=*/false, /*drawShadow=*/false, PlayerDrawMode::Normal, /*drawParticles=*/false);

    menu->Draw();

    int dummy1 = -1, dummy2 = -1;
    ButtonAction action = menu->HandleInput(dummy1, dummy2);

    if (input.IsPressed(KEY_INPUT_R)) action = ButtonAction::RESTART;

    if (input.IsPressed(KEY_INPUT_SPACE) && levelWasCompleted && !levelManager->IsLastLevel())
        action = ButtonAction::NEXT_LEVEL;

    if (action == ButtonAction::RESTART) {
        ApplyBlockTheme((levelManager->GetCurrentLevelId() - 1) / 10);
        levelManager->ReloadCurrentLevel();
        menu->SetState(GameState::PLAYING);
        FinishLevelStart();
    } else if (action == ButtonAction::BACK_TO_MENU) {
        audio.StopBgm();
        menu->ClearHistory();
        menu->SetState(GameState::MAIN_MENU);
        gameState = GameState::MAIN_MENU;
        if (menuBgMovie != -1) PlayMovieToGraph(menuBgMovie, DX_PLAYTYPE_LOOP);
        SetMouseDispFlag(TRUE);
    } else if (action == ButtonAction::NEXT_LEVEL && levelWasCompleted) {
        int nextId = levelManager->GetCurrentLevelId() + 1;
        ApplyBlockTheme((nextId - 1) / 10);
        levelManager->NextLevel();
        menu->SetState(GameState::PLAYING);
        FinishLevelStart();
    }
}

// Shared scene draw for every in-level state (PAUSED/PLAYING/DYING/CELEBRATING/RESULT):
// positions the camera, draws the skybox, stars, fog and level, then the player
// in whichever variant the calling state needs
void Game::DrawScene(bool debugModeForLevel, bool isDarkLevel, bool drawShadow,
                      PlayerDrawMode playerDrawMode, bool drawParticles) {
    camera.Apply(player.GetPos());
    VECTOR camPos = camera.GetPosition(player.GetPos());

    SetUseLighting(FALSE);
    SetUseBackCulling(FALSE);

    // Skybox (no fog)
    if (skyboxTextures[currentBiome] != -1) {
        DrawSkyboxSphere(player.GetPos(), skyboxTextures[currentBiome]);
    } else {
        DrawSpaceSkybox(player.GetPos(), currentBiome);
        DrawBiomePlanet(player.GetPos(), currentBiome);
    }

    // Regular levels: stars before geometry — platforms occlude them.
    // Dark levels draw them later instead, after the darkness overlay (ADD blending punches through the dark)
    if (!isDarkLevel) DrawStars(stars, player.GetPos());

    ApplyBiomeFog(currentBiome);
    Level* currentLevel = levelManager->GetCurrentLevel();
    if (currentLevel) currentLevel->Draw(debugModeForLevel);
    if (drawShadow) player.DrawShadow(currentLevel);
    if (playerDrawMode == PlayerDrawMode::FacingCamera) player.DrawFacingCamera(camPos);
    else player.Draw();
    if (drawParticles) particles.Draw();
    DisableFog();

    if (isDarkLevel) {
        if (darknessLevel > 0.01f) {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(darknessLevel * 240.0f));
            DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
        DrawStars(stars, player.GetPos());
    }
}

// ── Per-block platform theme ─────────────────────────────────────────────
void Game::ApplyBlockTheme(int blockIdx) {
    int theme = (blockIdx <= 1) ? 0 : (blockIdx <= 3) ? 1 : 2;
    // Biome maps 1:1 to the block (0-4)
    if (blockIdx != currentBiome) {
        currentBiome = blockIdx;
        stars = GenerateStars(1000, 400.0f, currentBiome);
    }
    if (theme == currentThemeGroup) return;
    currentThemeGroup = theme;
    LoadThemeModels(theme);
}

void Game::LoadThemeModels(int theme) {
    modelMgr.UnloadModel(ModelID::PLATFORM);
    modelMgr.UnloadModel(ModelID::FAKE_PLATFORM);
    modelMgr.UnloadModel(ModelID::CRUMBLING);
    modelMgr.UnloadModel(ModelID::MOVING);
    modelMgr.UnloadModel(ModelID::DISAPPEARING);
    modelMgr.UnloadModel(ModelID::ICE_PLATFORM);
    modelMgr.UnloadModel(ModelID::BOUNCE_PAD);

    const ThemeModelTable& table = THEME_MODEL_TABLES[theme];
    for (int i = 0; i < table.count; i++) {
        modelMgr.LoadModel(table.entries[i].id, table.entries[i].path, 1.0f);
    }
}

// Switches the theme pack (music+skyboxes) — called from the Theme Packs menu.
// If the player switches packs mid-run (pause -> settings), this immediately
// restarts the current block's BGM with the new track.
void Game::ApplyThemePack(int pack) {
    if (!profileMgr.IsThemePackUnlocked(pack)) return;
    profileMgr.SetThemePack(pack);
    LoadSkyboxPack(pack);
    audio.LoadBgm(PackFolder(pack));
    if (gameState == GameState::PLAYING || gameState == GameState::PAUSED) {
        int blockIdx = (levelManager->GetCurrentLevelId() - 1) / 10;
        audio.PlayBgm(blockIdx, menu->GetSettings().musicVolume);
    }
}

// Shared "tail" for start/restart/next-level — called AFTER the specific call
// site has already applied ApplyBlockTheme(...) and picked the right loading
// method (LoadLevel/RestartLevel/ReloadCurrentLevel/NextLevel), since those
// operations are semantically different (RestartLevel counts as a death,
// ReloadCurrentLevel doesn't) and don't collapse cleanly into one function.
void Game::FinishLevelStart() {
    player.Reset(levelManager->GetCurrentLevel()->GetPlayerSpawn());
    darknessLevel = 0.0f;
    gameState = GameState::PLAYING;
    if (menuBgMovie != -1) PauseMovieToGraph(menuBgMovie);
    audio.PlayBgm((levelManager->GetCurrentLevelId() - 1) / 10, menu->GetSettings().musicVolume);
    SetMouseDispFlag(FALSE);
}

// ── Theme packs (folder under media/: Defolt_Pack/Pack1/Pack2) ─────────────
std::wstring Game::PackFolder(int pack) const {
    switch (pack) {
        case 1:  return L"Pack1";
        case 2:  return L"Pack2";
        default: return L"Defolt_Pack";
    }
}

// Try JPG first, fall back to PNG
int Game::LoadSkybox(const wchar_t* jpgPath, const wchar_t* pngPath) {
    int h = LoadGraph(jpgPath);
    return (h != -1) ? h : LoadGraph(pngPath);
}

// Skybox textures (media/<Pack>/skybox/biomeN, -1 = file not found)
void Game::LoadSkyboxPack(int pack) {
    for (int i = 0; i < 5; i++) {
        if (skyboxTextures[i] != -1) { DeleteGraph(skyboxTextures[i]); skyboxTextures[i] = -1; }
    }
    std::wstring base = L"media/" + PackFolder(pack) + L"/skybox/biome";
    for (int i = 0; i < 5; i++) {
        std::wstring path = base + std::to_wstring(i);
        skyboxTextures[i] = LoadSkybox((path + L".jpg").c_str(), (path + L".png").c_str());
    }
}
