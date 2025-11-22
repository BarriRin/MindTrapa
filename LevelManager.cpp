#include "LevelManager.h"

LevelManager::LevelManager()
    : currentLevelId(1), totalLevels(20), deathCount(0), currentLevelTime(0.0f) {
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
    currentLevelTime = 0.0f; // Сброс таймера при загрузке уровня
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

void LevelManager::DrawLevelInfo(bool debugMode) const {
    // Всегда показываем базовую информацию
    DrawFormatString(10, 10, GetColor(255, 255, 255), L"Level: %d/%d", currentLevelId, totalLevels);
    DrawFormatString(10, 30, GetColor(255, 255, 255), L"Deaths: %d", deathCount);

    // Таймер (всегда показываем)
    int minutes = (int)(currentLevelTime / 60.0f);
    float seconds = currentLevelTime - (minutes * 60.0f);
    DrawFormatString(10, 50, GetColor(255, 255, 255), L"Time: %d:%05.2f", minutes, seconds);

    // Лучшее время (если есть)
    if (HasBestTime(currentLevelId)) {
        float bestTime = GetBestTime(currentLevelId);
        int bestMinutes = (int)(bestTime / 60.0f);
        float bestSeconds = bestTime - (bestMinutes * 60.0f);

        unsigned int bestColor = GetColor(100, 255, 100);
        if (currentLevelTime < bestTime) {
            bestColor = GetColor(255, 215, 0); // Золотой - бьём рекорд!
        }
        DrawFormatString(10, 70, bestColor, L"Best: %d:%05.2f", bestMinutes, bestSeconds);
    }

    // Tutorial подсказки (всегда показываем)
    if (currentLevelId == 6) {
        DrawFormatString(10, 110, GetColor(255, 255, 0), L"Get close to YELLOW button and press E!");
    }
    if (currentLevelId == 8) {
        DrawFormatString(10, 110, GetColor(255, 50, 50), L"WARNING: Platforms disappear 1.5 sec after touch!");
    }
    if (currentLevelId == 9) {
        DrawFormatString(10, 110, GetColor(255, 150, 50), L"Watch the spikes! They retract every 2 seconds!");
    }

    // Debug информация (только если F3 нажат)
    if (debugMode) {
        DrawFormatString(10, 130, GetColor(200, 200, 200), L"WASD - move, Space - jump, R - restart");
        DrawFormatString(10, 150, GetColor(200, 200, 200), L"E - activate button/switch");

        if (currentLevel) {
            DrawFormatString(10, 170, GetColor(255, 255, 0), L"Blocks in level: %d", (int)currentLevel->GetBlockCount());
        }
    }
}

void LevelManager::UpdateTimer(float deltaTime) {
    currentLevelTime += deltaTime;
}

void LevelManager::OnLevelComplete() {
    // Проверяем, побили ли мы рекорд
    if (!HasBestTime(currentLevelId) || currentLevelTime < bestTimes[currentLevelId]) {
        bestTimes[currentLevelId] = currentLevelTime;
    }
}

float LevelManager::GetBestTime(int levelId) const {
    auto it = bestTimes.find(levelId);
    if (it != bestTimes.end()) {
        return it->second;
    }
    return 0.0f;
}

bool LevelManager::HasBestTime(int levelId) const {
    return bestTimes.find(levelId) != bestTimes.end();
}