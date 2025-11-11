#include "LevelManager.h"

LevelManager::LevelManager()
    : currentLevelId(1), totalLevels(20), deathCount(0) {
    LoadLevel(1);
}

LevelManager::~LevelManager() {
    currentLevel.reset();
}

void LevelManager::LoadLevel(int levelId) {
    if (levelId < 1) levelId = 1;
    if (levelId > totalLevels) levelId = totalLevels;

    currentLevelId = levelId;
    currentLevel = std::make_unique<Level>(levelId);
}

void LevelManager::NextLevel() {
    currentLevelId++;
    if (currentLevelId > totalLevels) {
        currentLevelId = 1;
    }
    LoadLevel(currentLevelId);
}

void LevelManager::RestartLevel() {
    OnPlayerDeath();
    LoadLevel(currentLevelId);
}

void LevelManager::OnPlayerDeath() {
    deathCount++;
}

bool LevelManager::IsLastLevel() const {
    return currentLevelId >= totalLevels;
}

void LevelManager::DrawLevelInfo() const {
    DrawFormatString(10, 10, GetColor(255, 255, 255), L"Level: %d/%d", currentLevelId, totalLevels);
    DrawFormatString(10, 30, GetColor(255, 255, 255), L"Deaths: %d", deathCount);
    DrawFormatString(10, 50, GetColor(255, 255, 255), L"WASD - move, Space - jump, R - restart");
    DrawFormatString(10, 70, GetColor(255, 255, 255), L"E - activate button/switch");

    if (currentLevelId == 6) {
        DrawFormatString(10, 90, GetColor(255, 255, 0), L"Get close to YELLOW button and press E!");
    }
    if (currentLevelId == 8) {
        DrawFormatString(10, 90, GetColor(255, 50, 50), L"WARNING: Platforms disappear 1.5 sec after touch!");
    }
    if (currentLevelId == 9) {
        DrawFormatString(10, 90, GetColor(255, 150, 50), L"Watch the spikes! They retract every 2 seconds!");
    }

    if (currentLevel) {
        DrawFormatString(10, 110, GetColor(255, 255, 0), L"Blocks in level: %d", (int)currentLevel->GetBlockCount());
    }
}