#pragma once
#include "DxLib.h"

class Level;
class Camera;
class AudioManager;

// Player physics (movement, jumping, collision with the current Level),
// animation state, and death/celebration handling.
class Player {
public:
    Player();
    ~Player();

    // Loads the model and animations from a file
    bool LoadModel(const wchar_t* path);

    // Explicitly releases the DxLib model — call before DxLib_End(), otherwise
    // the destructor would run after DxLib is already torn down on exit
    void ReleaseModel();

    // Main update: input + physics + animation. Returns true if the player died.
    bool Update(float deltaTime, const Camera& camera, Level* level, AudioManager& audio, int soundVolume);

    // Draws the model (or a fallback cube)
    void Draw() const;

    // Draws the model facing a given point (the camera)
    void DrawFacingCamera(VECTOR camPos) const;

    // Draws the shadow under the player
    void DrawShadow(Level* level) const;

    // Starts the victory animation (Yes, one-shot)
    void StartCelebration();

    // Switches to the looping Wave animation (second phase of the victory sequence)
    void PlayWave();

    // Updates animation only (no physics — used for the DYING/CELEBRATING states)
    void UpdateAnimOnly(float deltaTime, bool looping = false);

    // Reset on level restart
    void Reset(VECTOR spawnPos);

    // State
    VECTOR GetPos()      const { return pos; }
    VECTOR GetVel()      const { return vel; }
    VECTOR GetSize()     const { return size; }
    bool   IsOnGround()  const { return onGround; }
    float  GetFacing()   const { return facingAngle; }
    bool   IsDead()      const { return dead; }

    void SetPos(VECTOR p) { pos = p; }
    void SetVel(VECTOR v) { vel = v; }

    // Used to orient the camera during the victory sequence
    float GetFacingToward(VECTOR target) const;

private:
    // Physics
    VECTOR pos;
    VECTOR vel;
    VECTOR horizVel;  // ice inertia (Block 5)
    VECTOR size;
    bool   onGround;
    bool   wasOnGround;
    bool   dead;
    float  facingAngle;
    float  iceFriction;
    VECTOR savedPlatVel;  // previous frame's platform velocity (for descending platforms)
    int    groundedStreak; // consecutive frames with onGround==true — damps animation jitter
                            // near platform edges, where the AABB's Y contact can flicker true/false frame to frame

    // Parameters
    static constexpr float SPEED         = 0.2f;
    static constexpr float JUMP_POWER    = 0.4f;
    static constexpr float GRAVITY       = -0.015f;
    static constexpr float MAX_FALL_SPEED = -0.6f;  // clamp on |vel.y| (symmetric for falling and reverse gravity) — otherwise high gravity can tunnel through thin colliders
    static constexpr float ANIM_FRAMERATE = 24.0f;  // assumed frame rate of the source model animation
    static constexpr float DEATH_FALL_Y   = -10.0f; // fall-death threshold below the level

    // Model and animations
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

    // Key debounce
    bool jumpPressed;
    bool activatePressed;

    void SwitchAnim(int animIdx);
    int  FindAnim(const wchar_t* substr) const;
    void ApplyAnimation(float deltaTime, bool looping);
};
