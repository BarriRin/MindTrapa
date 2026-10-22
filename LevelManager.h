#pragma once
#include "Level.h"
#include <memory>
#include <map>

// Owns the current Level and its transitions (load/restart/reload/next), plus
// per-level timing, death count, and best-time tracking.
class LevelManager {
private:
    std::unique_ptr<Level> currentLevel;
    int currentLevelId;
    int totalLevels;
    int deathCount;

    // Level timer
    float currentLevelTime;
    std::map<int, float> bestTimes; // Best time per level

public:
    LevelManager();
    ~LevelManager();

    // Explicitly releases the current level's DxLib models — call before DxLib_End(),
    // otherwise the destructor would run after DxLib is already torn down on exit
    void Shutdown();

    // Level management
    void LoadLevel(int levelId);
    void NextLevel();
    void RestartLevel();
    void ReloadCurrentLevel();

    // Getters
    Level* GetCurrentLevel() const { return currentLevel.get(); }
    int GetCurrentLevelId() const { return currentLevelId; }
    int GetTotalLevels() const { return totalLevels; }
    int GetDeathCount() const { return deathCount; }
    float GetCurrentLevelTime() const { return currentLevelTime; }
    float GetBestTime(int levelId) const;
    bool HasBestTime(int levelId) const;

    // Gameplay events
    void OnPlayerDeath();
    bool IsLastLevel() const;
    void OnLevelComplete(); // Called when the level is completed
    void UpdateTimer(float deltaTime); // Advances the timer

    // For the UI
    void DrawLevelInfo(bool debugMode = false) const;
};
