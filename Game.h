#pragma once

#include "DxLib.h"
#include "AudioManager.h"
#include "Camera.h"
#include "GameState.h"
#include "Input.h"
#include "LevelManager.h"
#include "Menu.h"
#include "ModelManager.h"
#include "ParticleSystem.h"
#include "Player.h"
#include "ProfileManager.h"
#include "Renderer.h"
#include <memory>
#include <string>
#include <vector>

// Owns every engine/game system and drives the Init -> Run -> Shutdown flow
// that used to live directly in WinMain.
class Game {
public:
    Game();

    bool Init();     // Engine + resource setup. Returns false if DxLib_Init fails.
    void Run();       // Main game loop.
    void Shutdown();  // Releases all DxLib resources. Call before DxLib_End().

private:
    enum class PlayerDrawMode { Normal, FacingCamera };

    // Engine singletons
    ModelManager&   modelMgr;
    ProfileManager& profileMgr;

    // Core systems. LevelManager/Menu load content and fonts through DxLib in
    // their constructors, so they can only be constructed once DxLib_Init()
    // has run (inside Init()) — hence unique_ptr instead of a value member.
    std::unique_ptr<LevelManager> levelManager;
    std::unique_ptr<Menu>         menu;
    AudioManager   audio;
    Player         player;
    Camera         camera;
    Input          input;
    ParticleSystem particles;

    // Biome / stars / skybox
    int                currentBiome      = 0;
    std::vector<Star>  stars;
    int                skyboxTextures[5] = { -1, -1, -1, -1, -1 };
    int                currentThemeGroup = 0;

    // Menu background video
    int menuBgMovie = -1;

    // Game state
    GameState gameState     = GameState::MAIN_MENU;
    float     darknessLevel = 0.0f;
    float     gameSpeed     = 1.0f;
    bool      debugMode     = false;
    bool      exitRequested = false;

    // Teleport debounce
    float teleportCooldown = 0.0f;

    // Result screen
    float dyingTimer        = 0.0f;
    bool  levelWasCompleted = false;

    // Victory celebration
    static constexpr float CELEBRATION_DURATION = 2.8f;
    float  celebrationTimer   = 0.0f;
    VECTOR celebrationFlagPos = VGet(0, 0, 0);
    int    pendingCelebStars  = 0;
    float  pendingCelebTime   = 0.0f;
    bool   pendingCelebIsLast = false;
    int    celebPhase         = 0;
    bool   secondBurstFired   = false;

    // Frame timer
    int lastFrameTime = 0;
    static constexpr float maxDeltaTime = 0.1f;

    // SFX preview while dragging the sound slider on the Settings screen
    int   lastPreviewSoundVolume = -1;
    bool  wasInSettings          = false;
    float sfxPreviewCooldown     = 0.0f;

    // Per-state update
    void UpdateMenu(float realDt);
    void UpdatePaused();
    void UpdatePlaying(float dt, float realDt);
    void UpdateDying(float dt);
    void UpdateCelebrating(float dt);
    void UpdateResult();

    // Shared scene draw (skybox, stars, fog, level, player) for all in-level states
    void DrawScene(bool debugModeForLevel, bool isDarkLevel, bool drawShadow,
                    PlayerDrawMode playerDrawMode, bool drawParticles);

    // Per-block platform theme and theme pack (music + skyboxes) helpers
    void ApplyBlockTheme(int blockIdx);
    void ApplyThemePack(int pack);
    void FinishLevelStart();
    std::wstring PackFolder(int pack) const;
    static int LoadSkybox(const wchar_t* jpgPath, const wchar_t* pngPath);
    void LoadSkyboxPack(int pack);
};
