#pragma once
#pragma once
#include "Level.h"
#include <memory>

class LevelManager {
private:
    std::unique_ptr<Level> currentLevel;
    int currentLevelId;
    int totalLevels;
    int deathCount;

public:
    LevelManager();
    ~LevelManager();

    // „T„„‚„p„r„|„u„~„y„u „…„‚„€„r„~„‘„}„y
    void LoadLevel(int levelId);
    void NextLevel();
    void RestartLevel();

    // „C„u„„„„„u„‚„
    Level* GetCurrentLevel() const { return currentLevel.get(); }
    int GetCurrentLevelId() const { return currentLevelId; }
    int GetTotalLevels() const { return totalLevels; }
    int GetDeathCount() const { return deathCount; }

    // „I„s„‚„€„r„„u „ƒ„€„q„„„„y„‘
    void OnPlayerDeath();
    bool IsLastLevel() const;

    // „D„|„‘ UI
    void DrawLevelInfo() const;
};