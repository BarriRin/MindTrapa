#pragma once
#include "DxLib.h"
#include <vector>
#include "ModelManager.h"

// Block types
enum class BlockType {
    PLATFORM = 0,      // Regular platform
    TRIGGER = 1,       // Advances to the next level
    INVISIBLE_WALL = 2,// Invisible wall (collides but isn't drawn)
    FAKE_PLATFORM = 3, // Fake platform (drawn but no collision)
    SPIKES = 4,        // Spikes (kill the player)
    DISAPPEARING = 5,  // Disappears after N seconds
    MOVING = 6,        // Moving platform
    BUTTON = 7,        // Button (triggers something)
    TELEPORT = 8,      // Teleport
    RETRACTABLE_SPIKES = 9, // Retract/extend on a timer [Block 2]
    CRUMBLING = 10,    // Falls apart after being touched [Block 2]
    FAKE_SPIKES = 11,  // Look dangerous but are harmless [Block 2]
    GRAVITY_ZONE = 12,     // Zone with modified gravity [Block 3]
    PENDULUM_BLADE = 13,   // Swinging axe blade (deadly) [Block 3]
    LIGHT_PULSE_ZONE = 14, // Flickering light zone (dark/light on a timer) [Block 4]
    ICE_PLATFORM = 15,    // Slippery platform (adds inertia to movement) [Block 5]
    BOUNCE_PAD = 16       // Launches the player upward [Block 5]
};

// Block struct
struct Block {
    VECTOR pos;         // Current position
    VECTOR size;        // Block size
    BlockType type;      // Block type

    // Extra parameters for mechanics
    VECTOR originalPos; // Starting position (for moving platforms)
    VECTOR prevPos;     // Previous position (used to derive velocity)
    float timer;        // For disappearing blocks / general timers
    bool isActive;      // For buttons and other active elements
    VECTOR moveTarget;  // For moving platforms (destination point)
    int linkId;         // For linked elements (button <-> door)
    float moveSpeed;    // Platform movement speed

    // 3D model parameters
    int modelHandle;    // Duplicated model handle (-1 if using primitives)
    VECTOR rotation;    // Model rotation
    VECTOR modelScale;  // Model scale (separate from size, which drives collision)
    bool useModel;      // Draw as a model or as a primitive

    // Block 3 mechanics parameters
    float gravityMultiplier; // Gravity multiplier (for GRAVITY_ZONE): 0.5=low, 2.0=high, -1.0=reverse
    float swingAngle;        // Current pendulum swing angle (for PENDULUM_BLADE)
    float swingSpeed;        // Pendulum swing speed
    float swingRange;        // Swing range in radians (e.g. PI/4 = +-45 deg)
    VECTOR pivotPoint;       // Pendulum pivot point (for PENDULUM_BLADE)

    // Block 5 mechanics parameters
    float frictionMultiplier; // Friction coefficient (for ICE_PLATFORM): 0.985 = smooth sliding (applied per frame with no input)

    // Constructor with defaults
    Block(VECTOR p, VECTOR s, BlockType t, int link = 0, bool active = true,
          VECTOR moveStart = VGet(0, 0, 0), VECTOR moveEnd = VGet(0, 0, 0),
          float speed = 1.0f, float tim = 0.0f)
        : pos(p), size(s), type(t),
          originalPos((t == BlockType::PENDULUM_BLADE) ? p : moveStart),
          prevPos(p),
          timer(t == BlockType::LIGHT_PULSE_ZONE && !active ? speed * 0.5f : tim),
          isActive(active), moveTarget(moveEnd), linkId(link), moveSpeed(speed),
          modelHandle(-1), rotation(VGet(0, 0, 0)),
          modelScale(VGet(1, 1, 1)), useModel(false),
          gravityMultiplier(1.0f), swingAngle(0.0f), swingSpeed(1.0f),
          swingRange(DX_PI_F / 3.0f), pivotPoint(VGet(0, 0, 0)),
          frictionMultiplier(0.985f) {}
};

// Level class
class Level {
private:
    std::vector<Block> blocks;
    VECTOR playerSpawn;
    int levelId;
    int blockId;        // Level-block ID (used for skybox selection)

    // Level data loading
    void LoadLevelData(int id);
    void LoadBlock1(int id);
    void LoadBlock2(int id);
    void LoadBlock3(int id);
    void LoadBlock4(int id);
    void LoadBlock5(int id);

    // Model initialization for blocks
    void InitializeModels();
    static ModelID BlockTypeToModelID(BlockType type);

public:
    Level(int id);
    ~Level();

    void Draw(bool debugMode) const;
    void DrawSkybox(VECTOR cameraPos) const;
    void Update(float deltaTime);

    // Collision and trigger checks
    bool CheckCollision(VECTOR playerPos, VECTOR playerSize, VECTOR& newPos, VECTOR& velocity, bool& onGround, VECTOR& platformVelocity, bool gravityReversed = false, bool* justBounced = nullptr);
    bool CheckWinTrigger(VECTOR playerPos, VECTOR playerSize) const;
    bool CheckDeadlyTrigger(VECTOR playerPos, VECTOR playerSize) const;
    bool CheckTeleportTrigger(VECTOR playerPos, VECTOR playerSize, VECTOR& teleportTarget) const;

    // Block 3 mechanics
    float CheckGravityZone(VECTOR playerPos, VECTOR playerSize) const; // Returns the gravity multiplier

    // Block 5 mechanics
    float CheckIcePlatform(VECTOR playerPos, VECTOR playerSize) const; // Returns frictionMultiplier (1.0 = not on ice)

    // Block 4 mechanics
    bool IsAnyLightPulseZoneActive() const; // true = light phase (at least one zone active)

    const std::vector<Block>& GetBlocks() const { return blocks; }
    VECTOR GetPlayerSpawn() const { return playerSpawn; }
    size_t GetBlockCount() const { return blocks.size(); }

    // Button activation
    void ActivateButton(VECTOR playerPos, VECTOR playerSize, bool keyPressed);
};
