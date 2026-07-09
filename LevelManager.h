#pragma once
#include "Level.h"
#include <memory>
#include <map>

class LevelManager {
private:
    std::unique_ptr<Level> currentLevel;
    int currentLevelId;
    int totalLevels;
    int deathCount;

    // Таймер уровня
    float currentLevelTime;
    std::map<int, float> bestTimes; // Лучшие времена для каждого уровня

public:
    LevelManager();
    ~LevelManager();

    // Явное освобождение DxLib-моделей текущего уровня — вызывать до DxLib_End(),
    // иначе деструктор сделает это уже после выгрузки DxLib при выходе из программы
    void Shutdown();

    // Управление уровнями
    void LoadLevel(int levelId);
    void NextLevel();
    void RestartLevel();
    void ReloadCurrentLevel();

    // Геттеры
    Level* GetCurrentLevel() const { return currentLevel.get(); }
    int GetCurrentLevelId() const { return currentLevelId; }
    int GetTotalLevels() const { return totalLevels; }
    int GetDeathCount() const { return deathCount; }
    float GetCurrentLevelTime() const { return currentLevelTime; }
    float GetBestTime(int levelId) const;
    bool HasBestTime(int levelId) const;

    // Игровые события
    void OnPlayerDeath();
    bool IsLastLevel() const;
    void OnLevelComplete(); // Вызывается при прохождении уровня
    void UpdateTimer(float deltaTime); // Обновление таймера

    // Для UI
    void DrawLevelInfo(bool debugMode = false) const;
};