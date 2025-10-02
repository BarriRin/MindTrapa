#pragma once
#include "DxLib.h"
#include <vector>

// „S„y„„ „q„|„€„{„€„r
enum class BlockType {
    PLATFORM = 0,      // „O„q„„‰„~„p„‘ „„|„p„„„†„€„‚„}„p
    TRIGGER = 1,       // „S„‚„y„s„s„u„‚ „„u„‚„u„‡„€„t„p „~„p „ƒ„|„u„t„…„„‹„y„z „…„‚„€„r„u„~„
    INVISIBLE_WALL = 2,// „N„u„r„y„t„y„}„p„‘ „ƒ„„„u„~„p („{„€„|„|„y„x„y„‘ „u„ƒ„„„, „~„€ „~„u „r„y„t„~„p)
    FAKE_PLATFORM = 3, // „U„p„|„„Š„y„r„p„‘ „„|„p„„„†„€„‚„}„p („r„y„t„~„p, „~„€ „{„€„|„|„y„x„y„y „~„u„„)
    SPIKES = 4,        // „Y„y„„ („…„q„y„r„p„„„ „y„s„‚„€„{„p)
    DISAPPEARING = 5,  // „I„ƒ„‰„u„x„p„„‹„p„‘ „„|„p„„„†„€„‚„}„p („‰„u„‚„u„x N „ƒ„u„{„…„~„t)
    MOVING = 6,        // „D„r„y„w„…„‹„p„‘„ƒ„‘ „„|„p„„„†„€„‚„}„p
    BUTTON = 7,        // „K„~„€„„{„p („p„{„„„y„r„y„‚„…„u„„ „‰„„„€-„„„€)
    TELEPORT = 8       // „S„u„|„u„„€„‚„„
};

// „R„„„‚„…„{„„„…„‚„p „q„|„€„{„p
struct Block {
    VECTOR pos;
    VECTOR size;
    BlockType type;

    // „D„€„„€„|„~„y„„„u„|„„~„„u „„p„‚„p„}„u„„„‚„ „t„|„‘ „}„u„‡„p„~„y„{
    float timer;        // „D„|„‘ „y„ƒ„‰„u„x„p„„‹„y„‡ „q„|„€„{„€„r, „„„p„z„}„u„‚„€„r
    bool isActive;      // „D„|„‘ „{„~„€„„€„{, „p„{„„„y„r„~„„‡ „„|„u„}„u„~„„„€„r
    VECTOR moveTarget;  // „D„|„‘ „t„r„y„w„…„‹„y„‡„ƒ„‘ „„|„p„„„†„€„‚„}
    int linkId;         // „D„|„‘ „ƒ„r„‘„x„p„~„~„„‡ „„|„u„}„u„~„„„€„r („{„~„€„„{„p¨„t„r„u„‚„)

    Block(VECTOR position, VECTOR blockSize, BlockType blockType,
        float time = 0.0f, bool active = true, VECTOR target = VGet(0, 0, 0), int link = 0)
        : pos(position), size(blockSize), type(blockType),
        timer(time), isActive(active), moveTarget(target), linkId(link) {
    }
};

// „K„|„p„ƒ„ƒ „…„‚„€„r„~„‘
class Level {
private:
    std::vector<Block> blocks;
    VECTOR playerSpawn;
    int levelId;

public:
    Level(int id);
    ~Level();

    // „O„ƒ„~„€„r„~„„u „}„u„„„€„t„
    void LoadLevelData(int id);
    void Update(float deltaTime); // „N„€„r„„z „}„u„„„€„t „t„|„‘ „€„q„~„€„r„|„u„~„y„‘ „}„u„‡„p„~„y„{!
    void Draw() const;
    bool CheckCollision(VECTOR playerPos, VECTOR playerSize, VECTOR& newPos, VECTOR& velocity, bool& onGround) const;
    bool CheckWinTrigger(VECTOR playerPos, VECTOR playerSize) const;
    bool CheckDeadlyTrigger(VECTOR playerPos, VECTOR playerSize) const; // „P„‚„€„r„u„‚„{„p „Š„y„„€„r
    bool CheckTeleportTrigger(VECTOR playerPos, VECTOR playerSize, VECTOR& teleportTarget) const; // „S„u„|„u„„€„‚„„„
    void ActivateButton(VECTOR playerPos, VECTOR playerSize, bool keyPressed); // „K„~„€„„{„y

    // „C„u„„„„„u„‚„
    VECTOR GetPlayerSpawn() const { return playerSpawn; }
    int GetId() const { return levelId; }
    size_t GetBlockCount() const { return blocks.size(); }

    // „D„|„‘ „€„„„|„p„t„{„y
    void PrintDebugInfo() const;
};