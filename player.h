#pragma once
#include "DxLib.h"

class Level;
class Camera;
class AudioManager;

class Player {
public:
    Player();
    ~Player();

    // Загружает модель и анимации из файла
    bool LoadModel(const wchar_t* path);

    // Основное обновление: ввод + физика + анимация. Возвращает true если игрок умер.
    bool Update(float deltaTime, const Camera& camera, Level* level, AudioManager& audio, int soundVolume);

    // Рисует модель (или запасной куб)
    void Draw() const;

    // Рисует модель повёрнутой лицом к указанной точке (камере)
    void DrawFacingCamera(VECTOR camPos) const;

    // Рисует тень под игроком
    void DrawShadow(Level* level) const;

    // Запускает анимацию победы
    void StartCelebration();

    // Обновляет только анимацию (без физики — для DYING/CELEBRATING состояний)
    void UpdateAnimOnly(float deltaTime, bool looping = false);

    // Сброс при рестарте уровня
    void Reset(VECTOR spawnPos);

    // Состояние
    VECTOR GetPos()      const { return pos; }
    VECTOR GetVel()      const { return vel; }
    VECTOR GetSize()     const { return size; }
    bool   IsOnGround()  const { return onGround; }
    float  GetFacing()   const { return facingAngle; }
    bool   IsDead()      const { return dead; }

    void SetPos(VECTOR p) { pos = p; }
    void SetVel(VECTOR v) { vel = v; }

    // Для отображения камеры во время праздника
    float GetFacingToward(VECTOR target) const;

private:
    // Физика
    VECTOR pos;
    VECTOR vel;
    VECTOR horizVel;  // инерция на льду (Block 5)
    VECTOR size;
    bool   onGround;
    bool   wasOnGround;
    bool   dead;
    float  facingAngle;
    float  iceFriction;

    // Параметры
    static constexpr float SPEED      = 0.2f;
    static constexpr float JUMP_POWER = 0.4f;
    static constexpr float GRAVITY    = -0.015f;

    // Модель и анимации
    int modelHandle;
    int animAttach;
    float animTime;
    int   currentAnim;

    int animIdle;
    int animRun;
    int animJump;
    int animFall;
    int animDeath;
    int animWave;
    int animYes;

    int celebPhase;

    // Антиспам клавиш
    bool jumpPressed;
    bool activatePressed;

    void SwitchAnim(int animIdx);
    int  FindAnim(const wchar_t* substr) const;
    void ApplyAnimation(float deltaTime, bool looping);
};
