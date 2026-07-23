#include "Level.h"

void Level::LoadBlock4(int id) {
    switch (id) {
    case 31: // Level 31 - Darkness tutorial: zigzag with varying height
        playerSpawn = VGet(0, 1, 0);

        // Starting platform
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Path: zigzag along Z, with wider gaps and varying height
        blocks.push_back(Block(VGet(8,  0,  2), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 1, -4), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 0,  3), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(29, 2, -3), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(36, 1,  1), VGet(3, 1, 3), BlockType::PLATFORM));

        // Finish
        blocks.push_back(Block(VGet(43, 1, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(45, 2,  0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 32: // Level 32 - Strict zigzag: wide spread in Z and height
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Zigzag: even indices — left side (Z=-5), odd — right side (Z=4)
        blocks.push_back(Block(VGet(8,  0, -5), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 2,  4), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 1, -5), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(29, 4,  4), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(36, 2, -4), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(43, 5,  3), VGet(3, 1, 3), BlockType::PLATFORM));

        // Finish
        blocks.push_back(Block(VGet(50, 5, -1), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(52, 6,  1), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 33: // Level 33 - Checkerboard: real/fake platforms interleaved
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // 5 columns x 3 rows (Z=-5, 0, +5), checkerboard pattern of real/fake
        // Column 1 (X=8, Y=0): L=REAL, C=FAKE, R=REAL
        blocks.push_back(Block(VGet(8,  0, -5), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8,  0,  0), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(8,  0,  4), VGet(3, 1, 3), BlockType::PLATFORM));

        // Column 2 (X=15, Y=1): L=FAKE, C=REAL, R=FAKE
        blocks.push_back(Block(VGet(15, 1, -5), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(15, 1,  0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 1,  4), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));

        // Column 3 (X=22, Y=2): L=REAL, C=FAKE, R=REAL
        blocks.push_back(Block(VGet(22, 2, -5), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 2,  0), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(22, 2,  5), VGet(3, 1, 3), BlockType::PLATFORM));

        // Column 4 (X=29, Y=1): L=FAKE, C=REAL, R=FAKE
        blocks.push_back(Block(VGet(29, 1, -5), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(29, 1,  0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(29, 1,  5), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));

        // Column 5 (X=36, Y=3): L=REAL, C=FAKE, R=REAL
        blocks.push_back(Block(VGet(36, 3, -4), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(36, 3,  1), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(36, 3,  5), VGet(3, 1, 3), BlockType::PLATFORM));

        // Finish
        blocks.push_back(Block(VGet(43, 3, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(45, 4,  0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 34: // Level 34 — Arc: right (+X) -> up (+Y) -> back (-X), 2 decoys
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(8, 1, 8), BlockType::PLATFORM));

        // Block 4 darkness zone
        blocks.push_back(Block(VGet(5, -2, -8), VGet(52, 11, 24), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 5.0f));

        // === SECTION 1: RIGHT (+X), Z~0, Y climbs slowly ===
        blocks.push_back(Block(VGet(12,  0,  0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22,  1,  0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22,  1, -6), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(32,  2,  0), VGet(4, 1, 4), BlockType::PLATFORM));

        // === PEAK: big jump up and to the right ===
        blocks.push_back(Block(VGet(44,  6,  1), VGet(5, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(51,  6,  1), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));

        // === SECTION 3: BACK (-X), Z~8, top=7 ===
        blocks.push_back(Block(VGet(34,  6,  7), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(24,  6,  7), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(14,  6,  7), VGet(4, 1, 4), BlockType::PLATFORM));

        // Finish
        blocks.push_back(Block(VGet( 4,  6,  7), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet( 6,  7, 10), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 35: // Level 35 — 7 islands in a spiral, pattern L-L-R-R-L-R-L
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(8, 1, 8), BlockType::PLATFORM));

        // Block 4 darkness zone
        blocks.push_back(Block(VGet(3, -3, -25), VGet(62, 13, 60), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 6.0f));

        // === ISLAND 1: center(12,0,0), LEFT=correct (linkId=21) ===
        blocks.push_back(Block(VGet( 9, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(10,  0, -2), VGet(2, 1, 2), BlockType::TELEPORT, 21));
        blocks.push_back(Block(VGet(13,  0,  1), VGet(2, 1, 2), BlockType::TELEPORT,  0));

        // === ISLAND 2: center(36,1,-20), LEFT=correct (linkId=22) ===
        blocks.push_back(Block(VGet(33,  0, -23), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(36,  0, -20), VGet(1, 1, 1), BlockType::TELEPORT, 21));
        blocks.push_back(Block(VGet(34,  1, -22), VGet(2, 1, 2), BlockType::TELEPORT, 22));
        blocks.push_back(Block(VGet(37,  1, -19), VGet(2, 1, 2), BlockType::TELEPORT,  0));

        // === ISLAND 3: center(56,2,-10), RIGHT=correct (linkId=23) ===
        blocks.push_back(Block(VGet(53,  1, -13), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(56,  1, -10), VGet(1, 1, 1), BlockType::TELEPORT, 22));
        blocks.push_back(Block(VGet(54,  2, -12), VGet(2, 1, 2), BlockType::TELEPORT,  0));
        blocks.push_back(Block(VGet(57,  2,  -9), VGet(2, 1, 2), BlockType::TELEPORT, 23));

        // === ISLAND 4: center(58,3,14), RIGHT=correct (linkId=24) ===
        blocks.push_back(Block(VGet(55,  2, 11), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(58,  2, 14), VGet(1, 1, 1), BlockType::TELEPORT, 23));
        blocks.push_back(Block(VGet(56,  3, 11), VGet(2, 1, 2), BlockType::TELEPORT,  0));
        blocks.push_back(Block(VGet(59,  3, 15), VGet(2, 1, 2), BlockType::TELEPORT, 24));

        // === ISLAND 5: center(38,4,30), LEFT=correct (linkId=25) ===
        blocks.push_back(Block(VGet(35,  3, 27), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(38,  3, 30), VGet(1, 1, 1), BlockType::TELEPORT, 24));
        blocks.push_back(Block(VGet(36,  4, 27), VGet(2, 1, 2), BlockType::TELEPORT, 25));
        blocks.push_back(Block(VGet(39,  4, 31), VGet(2, 1, 2), BlockType::TELEPORT,  0));

        // === ISLAND 6: center(16,5,22), RIGHT=correct (linkId=26) ===
        blocks.push_back(Block(VGet(13,  4, 19), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(16,  4, 22), VGet(1, 1, 1), BlockType::TELEPORT, 25));
        blocks.push_back(Block(VGet(14,  5, 19), VGet(2, 1, 2), BlockType::TELEPORT,  0));
        blocks.push_back(Block(VGet(17,  5, 23), VGet(2, 1, 2), BlockType::TELEPORT, 26));

        // === ISLAND 7: center(12,6,-10), LEFT=correct (linkId=27) ===
        blocks.push_back(Block(VGet( 9,  5, -13), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(12,  5, -10), VGet(1, 1, 1), BlockType::TELEPORT, 26));
        blocks.push_back(Block(VGet(10,  6, -13), VGet(2, 1, 2), BlockType::TELEPORT, 27));
        blocks.push_back(Block(VGet(13,  6,  -9), VGet(2, 1, 2), BlockType::TELEPORT,  0));

        // === FINISH ===
        blocks.push_back(Block(VGet(33,  7, -1), VGet(8, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(36,  7,  2), VGet(1, 1, 1), BlockType::TELEPORT, 27));
        blocks.push_back(Block(VGet(35,  8,  0), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 36: // Level 36 - LIGHT_PULSE + RETRACTABLE_SPIKES
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Flicker zone: 4s period
        blocks.push_back(Block(VGet(5, -1, -3), VGet(32, 10, 6), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 4.0f));

        // Corridor with retractable spikes
        blocks.push_back(Block(VGet(6, 0, -2), VGet(28, 1, 4), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(9, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(13, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));
        blocks.push_back(Block(VGet(17, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));
        blocks.push_back(Block(VGet(21, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 3.0f));
        blocks.push_back(Block(VGet(25, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.5f));

        blocks.push_back(Block(VGet(37, 1, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(39, 2, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 37: // Level 37 - Two segments: +X with jumps, then +Z with perpendicular blades
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, -2, -4), VGet(50, 18, 12), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 6.0f));

        // === Segment A: moving +X, corridor Z:-2.5..0.5, height climbs ===
        blocks.push_back(Block(VGet(7, 0, -1), VGet(5, 1, 3), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(13, 2, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(13, 9, -1);
        blocks.back().swingSpeed = 1.5f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        blocks.push_back(Block(VGet(16, 3, -1), VGet(4, 1, 3), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(22, 5, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(22, 11, -1);
        blocks.back().swingSpeed = 2.0f;
        blocks.back().swingRange = DX_PI_F / 2.5f;
        blocks.back().timer = 1.5f;

        blocks.push_back(Block(VGet(25, 6, -1), VGet(4, 1, 3), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(31, 7, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(31, 13, -1);
        blocks.back().swingSpeed = 1.8f;
        blocks.back().swingRange = DX_PI_F / 2.8f;
        blocks.back().timer = 2.5f;

        blocks.push_back(Block(VGet(33, 6, -1), VGet(4, 1, 9), BlockType::PLATFORM));  // Corner, Z:-1..8

        // === Segment B: moving +Z, corridor X:30..33, blades perpendicular ===
        blocks.push_back(Block(VGet(30, 7, 10), VGet(3, 1, 4), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(30.0f, 8, 16), VGet(3.0f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(31.5f, 14, 16);
        blocks.back().swingSpeed = 2.0f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        blocks.push_back(Block(VGet(30, 8, 19), VGet(3, 1, 4), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(30.0f, 9, 25), VGet(3.0f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(31.5f, 15, 25);
        blocks.back().swingSpeed = 1.7f;
        blocks.back().swingRange = DX_PI_F / 3.0f;
        blocks.back().timer = 1.5f;

        blocks.push_back(Block(VGet(28, 8, 28), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(30, 9, 30), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 38: // Level 38 - Zigzag at 0.3g: nonlinear path, can't be cleared in one jump
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(8, 1, 8), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(-4, 0, -6), VGet(69, 25, 20), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.3f;
        blocks.push_back(Block(VGet(-4, 0, -6), VGet(69, 25, 20), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 5.0f));

        // Zigzag: P1(+X) -> P2(+Z) -> P3(+X) -> P4(-Z) -> P5(+X) -> Finish
        blocks.push_back(Block(VGet(8,  1,  -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8,  4,  10), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 8,   8), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 11, -4), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(36, 14, -2), VGet(5, 1, 5), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(44, 14, -3), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(46, 15,  0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 39: // Level 39 - Large map: light reveals sections one at a time
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Section A: slow (10s)
        blocks.push_back(Block(VGet(7, -1, -5), VGet(24, 14, 10), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 10.0f));
        blocks.push_back(Block(VGet(8,  0, -3), VGet(4, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17, 2, -3), VGet(4, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(26, 4, -2), VGet(3, 1, 4), BlockType::CRUMBLING));

        // Section B: fast (3s)
        blocks.push_back(Block(VGet(31, -1, -5), VGet(24, 14, 10), BlockType::LIGHT_PULSE_ZONE,
            0, false, VGet(0,0,0), VGet(0,0,0), 3.0f));
        blocks.push_back(Block(VGet(33, 4, -3), VGet(4, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(41, 4, -3), VGet(3, 1, 4), BlockType::MOVING,
            0, true, VGet(41, 4, -3), VGet(41, 8, -3), 1.5f));
        blocks.push_back(Block(VGet(47, 4, -3), VGet(4, 1, 6), BlockType::PLATFORM));

        // Section C: medium (6s), final push
        blocks.push_back(Block(VGet(55, -1, -5), VGet(28, 14, 10), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 6.0f));
        blocks.push_back(Block(VGet(57, 4, -3), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(65, 6, -2), VGet(3, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(73, 7, -3), VGet(4, 1, 4), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(82, 4, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(84, 5, 0),  VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 40: // Level 40 - Block 4 FINALE: everything at once!
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === SECTION 1: Flicker + moving platform ===
        blocks.push_back(Block(VGet(7, -1, -4), VGet(22, 12, 8), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 5.0f));
        blocks.push_back(Block(VGet(9,  0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 1, -2), VGet(4, 1, 3), BlockType::MOVING,
            0, true, VGet(15, 1, -2), VGet(23, 1, -2), 2.0f));

        // === SECTION 2: Flicker + retractable spikes ===
        blocks.push_back(Block(VGet(29, -1, -4), VGet(18, 10, 8), BlockType::LIGHT_PULSE_ZONE,
            0, false, VGet(0,0,0), VGet(0,0,0), 4.0f));
        blocks.push_back(Block(VGet(30, 0, -2), VGet(14, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(32, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(36, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));
        blocks.push_back(Block(VGet(40, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));

        // === SECTION 3: 0.35x gravity + flicker + BLADE PERPENDICULAR TO TRAVEL ===
        blocks.push_back(Block(VGet(46, -1, -5), VGet(12, 18, 48), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.35f;
        blocks.push_back(Block(VGet(46, -1, -5), VGet(12, 18, 48), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 6.0f));
        blocks.push_back(Block(VGet(48, 0, -2), VGet(5, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(48, 2, 10), VGet(5, 1, 5), BlockType::PLATFORM));

        // Blade — X-swing covers the corridor X:48..53, player moves in +Z
        blocks.push_back(Block(VGet(50.5f, 3, 17), VGet(6, 2, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(50.5f, 14, 17);
        blocks.back().swingSpeed = 1.8f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        blocks.push_back(Block(VGet(48, 3, 20), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(48, 5, 27), VGet(5, 1, 5), BlockType::CRUMBLING));

        // === FINISH ===
        blocks.push_back(Block(VGet(46, 5, 35), VGet(7, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(49, 6, 40), VGet(2, 1, 2), BlockType::TRIGGER));
        break;
    }
}
