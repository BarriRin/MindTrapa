#include "Level.h"

void Level::LoadBlock1(int id) {
    switch (id) {
    case 1: // Level 1 - simple tutorial
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(10, 1, 10), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 2, 0), VGet(2, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 2: // Level 2 - jumps + first spikes!
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 1, -2), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(12, 3, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 2, 0), VGet(1, 1, 1), BlockType::SPIKES));
        blocks.push_back(Block(VGet(15, 4, 2), VGet(1, 1, 1), BlockType::SPIKES));
        blocks.push_back(Block(VGet(20, 2, 0), VGet(2, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(20, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 3: // Level 3 - invisible walls
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, -1, -3), VGet(2, 8, 10), BlockType::INVISIBLE_WALL)); // Y:-1..7 — too tall to jump over (max jump Y=6.3)
        blocks.push_back(Block(VGet(10, 1, 0), VGet(4, 1, 4), BlockType::PLATFORM));  // P1: behind the wall, gap=7
        blocks.push_back(Block(VGet(2, 1, 8), VGet(4, 1, 4), BlockType::PLATFORM));   // P2: detour (Z>7 — around the wall), gap=5
        blocks.push_back(Block(VGet(10, 1, 8), VGet(4, 1, 4), BlockType::PLATFORM));  // P3: gap=4 from P2
        blocks.push_back(Block(VGet(15, 0, 0), VGet(2, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 4: // Level 4 - teleports
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(2, 1, 2), BlockType::TELEPORT, 1));
        blocks.push_back(Block(VGet(20, 3, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(21, 4, 1), VGet(2, 1, 2), BlockType::TELEPORT, 1));
        blocks.push_back(Block(VGet(30, 5, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(32, 6, 1), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 5: // Level 5 - fake teleports
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, -3), VGet(2, 1, 2), BlockType::TELEPORT, 1));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(2, 1, 2), BlockType::TELEPORT, 2));
        blocks.push_back(Block(VGet(8, 1, 3), VGet(2, 1, 2), BlockType::TELEPORT, 3));
        blocks.push_back(Block(VGet(20, 1, -3), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 2, -3), VGet(1, 1, 1), BlockType::SPIKES));
        blocks.push_back(Block(VGet(20, 1, 3), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(25, 0, 3), VGet(2, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(25, 1, 3), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 6: // Level 6 - buttons and doors
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 0, 0), VGet(2, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(2, 1, 2), BlockType::BUTTON, 10, false));
        blocks.push_back(Block(VGet(15, 1, 0), VGet(4, 1, 4), BlockType::DISAPPEARING, 10, false));
        blocks.push_back(Block(VGet(22, 0, 0), VGet(2, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 7: // Level 7 - moving platforms
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 1, 0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(10, 2, 0), VGet(4, 1, 4), BlockType::MOVING, 0, true, VGet(10, 2, 0), VGet(20, 2, 0)));
        blocks.push_back(Block(VGet(25, 3, 0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(30, 3, 0), VGet(2, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(30, 4, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 8: // Level 8 - disappearing platforms
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 2, 0), VGet(3, 1, 3), BlockType::DISAPPEARING, 0, true));
        blocks.push_back(Block(VGet(10, 3, 0), VGet(3, 1, 3), BlockType::DISAPPEARING, 0, true));
        blocks.push_back(Block(VGet(15, 4, 0), VGet(2, 1, 2), BlockType::DISAPPEARING, 0, true));
        blocks.push_back(Block(VGet(20, 5, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(25, 4, 0), VGet(2, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(25, 5, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 9: // Level 9 - retractable spikes
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Section 1 - narrow corridor
        blocks.push_back(Block(VGet(5, 0, -1), VGet(12, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(7, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1));
        blocks.push_back(Block(VGet(10, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));
        blocks.push_back(Block(VGet(13, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1));
        blocks.push_back(Block(VGet(16, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));

        // Section 2 - a bit wider
        blocks.push_back(Block(VGet(20, 2, -2), VGet(8, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(21, 3, -2), VGet(1, 1, 5), BlockType::SPIKES, 1));
        blocks.push_back(Block(VGet(23, 3, -2), VGet(1, 1, 5), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));
        blocks.push_back(Block(VGet(25, 3, -2), VGet(1, 1, 5), BlockType::SPIKES, 1));
        blocks.push_back(Block(VGet(27, 3, -2), VGet(1, 1, 5), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));

        // Section 3 - final push
        blocks.push_back(Block(VGet(31, 3, -1), VGet(10, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(32, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 1));
        blocks.push_back(Block(VGet(34, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.5f));
        blocks.push_back(Block(VGet(36, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));
        blocks.push_back(Block(VGet(38, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.5f));
        blocks.push_back(Block(VGet(40, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));

        // Finish
        blocks.push_back(Block(VGet(44, 4, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(46, 5, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 10: // Level 10 - BLOCK 1 FINALE: spikes → moving → teleport → disappearing
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === SECTION 1: Retractable spikes — timing ===
        blocks.push_back(Block(VGet(7, 0, -1), VGet(14, 1, 3), BlockType::PLATFORM));  // gap=4 from start
        blocks.push_back(Block(VGet(9,  1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(12, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.5f));
        blocks.push_back(Block(VGet(16, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 0.75f));

        // === SECTION 2: Moving platform — the only way across the gap ===
        blocks.push_back(Block(VGet(23, 0, -1), VGet(3, 1, 3), BlockType::PLATFORM));  // breather, gap=2
        blocks.push_back(Block(VGet(28, 0, -1), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(28, 0, -1), VGet(42, 0, -1), 2.0f));
        blocks.push_back(Block(VGet(47, 0, -1), VGet(5, 1, 3), BlockType::PLATFORM));  // gap=2 from end of travel

        // === SECTION 3: Teleport — 12-unit gap, can't be jumped ===
        blocks.push_back(Block(VGet(49, 1, 0),  VGet(2, 1, 2), BlockType::TELEPORT, 10));
        blocks.push_back(Block(VGet(64, 0, -1), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(66, 1, 0),  VGet(2, 1, 2), BlockType::TELEPORT, 10));

        // === SECTION 4: Disappearing platforms — keep moving ===
        blocks.push_back(Block(VGet(74, 0, 0), VGet(3, 1, 3), BlockType::DISAPPEARING, 0, true));  // gap=5
        blocks.push_back(Block(VGet(81, 0, 0), VGet(3, 1, 3), BlockType::DISAPPEARING, 0, true));  // gap=4
        blocks.push_back(Block(VGet(88, 0, 0), VGet(3, 1, 3), BlockType::DISAPPEARING, 0, true));  // gap=4

        // === FINISH ===
        blocks.push_back(Block(VGet(96, 0, -1), VGet(5, 1, 5), BlockType::PLATFORM));  // gap=5
        blocks.push_back(Block(VGet(98, 1, 0),  VGet(2, 1, 2), BlockType::TRIGGER));
        break;
    }
}
