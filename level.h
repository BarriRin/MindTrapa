#pragma once
#pragma once
#include "DxLib.h"
#include <vector>

struct Block {
    VECTOR pos;
    VECTOR size;
    int type; // 0-„€„q„„‰„~„„z, 1-„„„‚„y„s„s„u„‚, 2-„~„u„r„y„t„y„}„„z „y „„.„t.
    int color;
};

class Level {
public:
    std::vector<Block> blocks;
    VECTOR playerStart;
    int levelId;

    void LoadLevel(int id);
    void Draw();
    void CheckCollisions(VECTOR playerPos, VECTOR playerSize);
    bool CheckWinTrigger(VECTOR playerPos);
};