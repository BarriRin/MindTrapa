#pragma once
class GameManager {
    Level currentLevel;
    int currentLevelId;
    int deaths;

public:
    void LoadLevel(int id);
    void RestartLevel();
    void NextLevel();
    void Update();
};