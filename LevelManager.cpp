#include "LevelManager.h"
#include "ProfileManager.h"
#include "Localization.h"

LevelManager::LevelManager()
    : currentLevelId(1), totalLevels(50), deathCount(0), currentLevelTime(0.0f) {
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

void LevelManager::ReloadCurrentLevel() {
    LoadLevel(currentLevelId);
}

void LevelManager::OnPlayerDeath() {
    deathCount++;
}

bool LevelManager::IsLastLevel() const {
    return currentLevelId >= totalLevels;
}

void LevelManager::DrawLevelInfo(bool debugMode) const {
    Localization& loc = Localization::GetInstance();
    int fontNormal = loc.GetFont(FontSize::Normal);

    // Всегда показываем базовую информацию
    DrawFormatStringToHandle(10, 10, GetColor(255, 255, 255), fontNormal, loc.Tr(StrId::HUD_LEVEL), currentLevelId, totalLevels);
    DrawFormatStringToHandle(10, 30, GetColor(255, 255, 255), fontNormal, loc.Tr(StrId::HUD_DEATHS), deathCount);

    // Таймер (всегда показываем)
    int minutes = (int)(currentLevelTime / 60.0f);
    float seconds = currentLevelTime - (minutes * 60.0f);
    DrawFormatStringToHandle(10, 50, GetColor(255, 255, 255), fontNormal, loc.Tr(StrId::TIME_LABEL), minutes, seconds);

    // Лучшее время (если есть)
    if (HasBestTime(currentLevelId)) {
        float bestTime = GetBestTime(currentLevelId);
        int bestMinutes = (int)(bestTime / 60.0f);
        float bestSeconds = bestTime - (bestMinutes * 60.0f);

        unsigned int bestColor = GetColor(100, 255, 100);
        if (currentLevelTime < bestTime) {
            bestColor = GetColor(255, 215, 0); // Золотой - бьём рекорд!
        }
        DrawFormatStringToHandle(10, 70, bestColor, fontNormal, loc.Tr(StrId::HUD_BEST), bestMinutes, bestSeconds);
    }

    // Tutorial подсказки (всегда показываем)
    if (currentLevelId == 6) {
        DrawStringToHandle(10, 110, loc.Tr(StrId::HUD_HINT_BUTTON), GetColor(255, 255, 0), fontNormal);
    }
    if (currentLevelId == 8) {
        DrawStringToHandle(10, 110, loc.Tr(StrId::HUD_HINT_CRUMBLE), GetColor(255, 50, 50), fontNormal);
    }
    if (currentLevelId == 9) {
        DrawStringToHandle(10, 110, loc.Tr(StrId::HUD_HINT_SPIKES), GetColor(255, 150, 50), fontNormal);
    }

    // Debug информация (только если F3 нажат)
    if (debugMode) {
        DrawStringToHandle(10, 130, loc.Tr(StrId::HUD_CONTROLS), GetColor(200, 200, 200), fontNormal);
        DrawStringToHandle(10, 150, loc.Tr(StrId::HUD_BUTTON_HINT), GetColor(200, 200, 200), fontNormal);

        if (currentLevel) {
            DrawFormatStringToHandle(10, 170, GetColor(255, 255, 0), fontNormal, loc.Tr(StrId::HUD_BLOCK_COUNT), (int)currentLevel->GetBlockCount());
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
    // Profile has persistent data; fall back to session cache
    const ProfileManager& pm = ProfileManager::GetInstance();
    if (pm.HasBestTime(levelId)) return pm.GetBestTime(levelId);
    auto it = bestTimes.find(levelId);
    return (it != bestTimes.end()) ? it->second : 0.0f;
}

bool LevelManager::HasBestTime(int levelId) const {
    if (ProfileManager::GetInstance().HasBestTime(levelId)) return true;
    return bestTimes.find(levelId) != bestTimes.end();
}