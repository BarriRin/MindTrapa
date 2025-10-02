#include "LevelManager.h"

LevelManager::LevelManager()
    : currentLevelId(1), totalLevels(6), deathCount(0) {  // „T„r„u„|„y„‰„y„|„y „t„€ 6 „…„‚„€„r„~„u„z
    LoadLevel(1);
}

LevelManager::~LevelManager() {
    currentLevel.reset();
}

void LevelManager::LoadLevel(int levelId) {
    // „O„s„‚„p„~„y„‰„y„r„p„u„} ID „…„‚„€„r„~„‘
    if (levelId < 1) levelId = 1;
    if (levelId > totalLevels) levelId = totalLevels;

    currentLevelId = levelId;

    // „R„€„x„t„p„u„} „~„€„r„„z „…„‚„€„r„u„~„
    currentLevel = std::make_unique<Level>(levelId);
}

void LevelManager::NextLevel() {
    currentLevelId++;

    if (currentLevelId > totalLevels) {
        // „I„s„‚„p „„‚„€„z„t„u„~„p! „M„€„w„~„€ „„€„{„p„x„p„„„ credits „y„|„y „„u„‚„u„x„p„„…„ƒ„„„y„„„
        currentLevelId = 1; // „P„€„{„p „„‚„€„ƒ„„„€ „„u„‚„u„x„p„„…„ƒ„{„p„u„}
    }

    LoadLevel(currentLevelId);
}

void LevelManager::RestartLevel() {
    OnPlayerDeath(); // „T„r„u„|„y„‰„y„r„p„u„} „ƒ„‰„u„„„‰„y„{ „ƒ„}„u„‚„„„u„z
    LoadLevel(currentLevelId); // „P„u„‚„u„x„p„s„‚„…„w„p„u„} „„„u„{„…„‹„y„z „…„‚„€„r„u„~„
}

void LevelManager::OnPlayerDeath() {
    deathCount++;
}

bool LevelManager::IsLastLevel() const {
    return currentLevelId >= totalLevels;
}

void LevelManager::DrawLevelInfo() const {
    // UI „y„~„†„€„‚„}„p„ˆ„y„‘
    DrawFormatString(10, 10, GetColor(255, 255, 255), L"Level: %d/%d", currentLevelId, totalLevels);
    DrawFormatString(10, 30, GetColor(255, 255, 255), L"Deaths: %d", deathCount);
    DrawFormatString(10, 50, GetColor(255, 255, 255), L"WASD - move, Space - jump, R - restart");
    DrawFormatString(10, 70, GetColor(255, 255, 255), L"E - activate button/switch");

    // „P„€„t„ƒ„{„p„x„{„y „t„|„‘ „…„‚„€„r„~„u„z
    if (currentLevelId == 6) {
        DrawFormatString(10, 90, GetColor(255, 255, 0), L"Get close to YELLOW button and press E!");
    }

    // „O„„„|„p„t„€„‰„~„p„‘ „y„~„†„€„‚„}„p„ˆ„y„‘ („r„‚„u„}„u„~„~„€)
    if (currentLevel) {
        DrawFormatString(10, 120, GetColor(255, 255, 0), L"Blocks in level: %d", (int)currentLevel->GetBlockCount());
    }
}