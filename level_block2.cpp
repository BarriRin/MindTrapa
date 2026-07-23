#include "Level.h"

void Level::LoadBlock2(int id) {
    switch (id) {
    case 11: // Level 11 - RETRACTABLE_SPIKES: 4 sections, turn toward +Z
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Section 1 - one slow spike (tutorial)
        // Platform X:5..17, Z:-2..3, gap=2 from start
        blocks.push_back(Block(VGet(5, 0, -2), VGet(12, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(9, 1, -2), VGet(3, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));

        // Section 2 - two spikes out of phase (gap=2)
        // When A is up, B is retracted, and vice versa
        blocks.push_back(Block(VGet(19, 0, -2), VGet(14, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(21, 1, -2), VGet(3, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(26, 1, -2), VGet(3, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));

        // Turn toward +Z - two crosswise spikes block the path
        // Platform X:35..40, Z:-3..15 (gap=2 from section 2)
        blocks.push_back(Block(VGet(35, 0, -3), VGet(5, 1, 18), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(35, 1,  1), VGet(5, 1, 3), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f)); // Z:1..4
        blocks.push_back(Block(VGet(35, 1,  8), VGet(5, 1, 3), BlockType::RETRACTABLE_SPIKES, 0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f)); // Z:8..11

        // Section 3 - three fast spikes (speed=2.0, wave pattern), gap=2 in Z from the turn
        // Platform X:33..45, Z:17..22
        blocks.push_back(Block(VGet(33, 0, 17), VGet(12, 1, 5), BlockType::PLATFORM));
        // Three spikes: A covers the left edge (X:33..37), B the middle (X:37..41), C the right edge (X:41..45)
        // Spikes span the full platform width - no way around at the edges
        blocks.push_back(Block(VGet(33, 1, 17), VGet(4, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true, VGet(0,0,0), VGet(0,0,0), 2.0f, 0.00f)); // 2s cycle
        blocks.push_back(Block(VGet(37, 1, 17), VGet(4, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true, VGet(0,0,0), VGet(0,0,0), 2.0f, 0.67f)); // offset by 1/3 cycle
        blocks.push_back(Block(VGet(41, 1, 17), VGet(4, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true, VGet(0,0,0), VGet(0,0,0), 2.0f, 1.33f)); // offset by 2/3 cycle

        // Finish (gap=2 in Z, width matches the whole section so it catches the player from any lane)
        blocks.push_back(Block(VGet(33, 0, 24), VGet(12, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(37, 1, 28), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 12: // Level 12 - CRUMBLING: 8 platforms in an L-shape, fast crumble (0.3s)
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Phase 1: +X direction (4 platforms), speed=0.6 -> 0.3s until it drops
        // gap=2 from start, then gap=3 between each platform
        blocks.push_back(Block(VGet(5,  0, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));
        blocks.push_back(Block(VGet(11, 0, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));
        blocks.push_back(Block(VGet(17, 0, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));
        blocks.push_back(Block(VGet(23, 0, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));

        // Turn toward +Z (jump from P4 to P5, gap=3 in Z, Y+1)
        blocks.push_back(Block(VGet(23, 1,  5), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f)); // X:23..26 = full overlap with P4
        blocks.push_back(Block(VGet(21, 1, 11), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));
        blocks.push_back(Block(VGet(20, 2, 17), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));
        blocks.push_back(Block(VGet(19, 2, 23), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));

        // Safe landing (gap=3 in Z from P8)
        blocks.push_back(Block(VGet(15, 2, 29), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17, 3, 32), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 13: // Level 13 - Two moving platforms back to back + a corridor of 3 spikes
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Intermediate landing - boards the first moving platform (gap=2)
        blocks.push_back(Block(VGet(5, 0, -1), VGet(4, 1, 3), BlockType::PLATFORM));

        // Moving platform A (slow, Y=0, gap=2 from the landing)
        // When A reaches X=24, its right edge X=27 lines up with the next landing - safe to step off
        blocks.push_back(Block(VGet(11, 0, 0), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(11, 0, 0), VGet(24, 0, 0), 1.5f));

        // Landing between the two moving platforms (wait here for the second one)
        blocks.push_back(Block(VGet(27, 0, -1), VGet(5, 1, 4), BlockType::PLATFORM));

        // Moving platform B (faster, Y=1, gap=2 from the landing)
        // When B reaches X=46, its right edge X=49 lines up with the final platform - step off
        blocks.push_back(Block(VGet(34, 1, 0), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(34, 1, 0), VGet(46, 1, 0), 2.0f));

        // Final platform: wide landing zone + a corridor with 3 spikes
        blocks.push_back(Block(VGet(49, 1, -2), VGet(15, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(51, 2, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(55, 2, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));
        blocks.push_back(Block(VGet(59, 2, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));

        // Finish (gap=2 from the end of the final platform)
        blocks.push_back(Block(VGet(66, 1, -2), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(68, 2,  0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 14: { // Level 14 - Subverting expectations: the "clean" path kills you, the "scary" pink one is safe
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Spike fields - 1x1x1 tiles with a model (not a primitive): fields are smaller than they
        // used to be (see below), but the model on each tile no longer stretches or distorts
        auto redSpikes = [this](int x0, int x1, int z0, int z1) {
            for (int tx = x0; tx < x1; tx++)
                for (int tz = z0; tz < z1; tz++)
                    blocks.push_back(Block(VGet((float)tx, 1.f, (float)tz), VGet(1, 1, 1), BlockType::SPIKES));
        };
        auto pinkSpikes = [this](int x0, int x1, int z0, int z1) {
            for (int tx = x0; tx < x1; tx++)
                for (int tz = z0; tz < z1; tz++)
                    blocks.push_back(Block(VGet((float)tx, 1.f, (float)tz), VGet(1, 1, 1), BlockType::FAKE_SPIKES));
        };

        // === SECTION 1: The fork ===
        blocks.push_back(Block(VGet(5, 0, -7), VGet(5, 1, 16), BlockType::PLATFORM));

        // PATH A - looks clean, but runs into real spikes (dead end, empty space beyond)
        blocks.push_back(Block(VGet(10, 0, 5), VGet(5, 1, 5), BlockType::PLATFORM));
        redSpikes(13, 15, 5, 10);

        // PATH B - floor entirely covered in pink spikes - safe
        blocks.push_back(Block(VGet(10, 0, -8), VGet(5, 1, 5), BlockType::PLATFORM));
        pinkSpikes(10, 15, -8, -3);

        // Landing (jump across a gap of 4 - unchanged; its Z range only lines up with PATH B,
        // so trying to jump the spikes on PATH A and land here overshoots into empty space)
        blocks.push_back(Block(VGet(19, 0, -9), VGet(5, 1, 6), BlockType::PLATFORM));

        // === SECTION 2: The open field ===
        blocks.push_back(Block(VGet(28, 0, -4), VGet(12, 1, 9), BlockType::PLATFORM));
        // Real spikes block the direct route to the exit (whole depth of the field except the pink strip)
        redSpikes(36, 40, -4, 3);
        // Pink strip along the far wall - the only safe way through
        pinkSpikes(28, 40, 3, 5);

        // Exit from section 2 - its Z range lines up ONLY with the pink strip (3..5), not the
        // full width of the field, otherwise the spikes could just be jumped over and landed past
        blocks.push_back(Block(VGet(40, 0, 3), VGet(5, 1, 2), BlockType::PLATFORM));

        // === SECTION 3: Finale - two lanes ===
        blocks.push_back(Block(VGet(49, 0, -2), VGet(9, 1, 12), BlockType::PLATFORM));
        // Clean-looking lane - runs into real spikes near the end
        redSpikes(54, 58, -2, 3);
        // Pink lane runs the full length - safe, leads to the finish
        pinkSpikes(49, 58, 5, 9);
        // Landing past the spikes + finish flag (Z range lines up only with the pink lane)
        blocks.push_back(Block(VGet(58, 0, 5), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(59, 1, 6), VGet(2, 1, 2), BlockType::TRIGGER));
        break;
    }

    case 15: // Level 15 - CRUMBLING + timing: gap=3 in X, turn toward +Z with gap=4
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-3, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));

        // +X phase: gap=2 from start, then gap=3 between platforms, height +1 every other one
        blocks.push_back(Block(VGet(5,  0, -1), VGet(3, 1, 3), BlockType::CRUMBLING)); // gap=2 from start
        blocks.push_back(Block(VGet(11, 1, -1), VGet(3, 1, 3), BlockType::CRUMBLING)); // gap=3, Y+1
        blocks.push_back(Block(VGet(17, 1, -1), VGet(3, 1, 3), BlockType::CRUMBLING)); // gap=3
        blocks.push_back(Block(VGet(23, 2, -1), VGet(3, 1, 3), BlockType::CRUMBLING)); // gap=3, Y+1

        // Turn toward +Z: gap=4 in Z from the last +X platform (Z:-1..2 -> Z:6..9)
        blocks.push_back(Block(VGet(22, 2,  6), VGet(3, 1, 3), BlockType::CRUMBLING)); // Z gap=4, X overlap ✓
        blocks.push_back(Block(VGet(21, 3, 13), VGet(3, 1, 3), BlockType::CRUMBLING)); // Z gap=4, Y+1

        // Finish (gap=4 in Z from the last platform)
        blocks.push_back(Block(VGet(18, 3, 20), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(20, 4, 22), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 16: // Level 16 - Invisible corridor: invisible walls narrow the path, spikes demand timing
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === CORRIDOR 1: +X direction ===
        // Platform X:5..23, Z:-1..2 (3 units wide)
        blocks.push_back(Block(VGet(5, 0, -1), VGet(18, 1, 3), BlockType::PLATFORM));
        // Invisible walls bound the corridor in Z
        // Start Y=1 (floor surface), height=8 -> top Y=9, above the ~6.3 max jump height from the floor
        blocks.push_back(Block(VGet(5, 1, -3), VGet(18, 8, 2), BlockType::INVISIBLE_WALL)); // south wall (Z:-3..-1)
        blocks.push_back(Block(VGet(5, 1,  2), VGet(18, 8, 2), BlockType::INVISIBLE_WALL)); // north wall (Z:2..4)
        // Three spikes in a wave, spanning the full corridor width (size.z=3)
        blocks.push_back(Block(VGet( 9, 1, -1), VGet(2, 1, 3), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(14, 1, -1), VGet(2, 1, 3), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));
        blocks.push_back(Block(VGet(19, 1, -1), VGet(2, 1, 3), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));

        // === REST AREA: turn toward +Z ===
        // X:22..28, Z:-1..9 - connects the end of corridor 1 to the start of corridor 2
        blocks.push_back(Block(VGet(22, 0, -1), VGet(6, 1, 10), BlockType::PLATFORM));

        // === CORRIDOR 2: +Z direction ===
        // Platform X:23..26, Z:8..23 (3 units wide)
        blocks.push_back(Block(VGet(23, 0, 8), VGet(3, 1, 15), BlockType::PLATFORM));
        // Invisible walls in X (Y=1, height=8 -> top Y=9, above max jump height)
        blocks.push_back(Block(VGet(21, 1, 8), VGet(2, 8, 15), BlockType::INVISIBLE_WALL)); // west wall (X:21..23)
        blocks.push_back(Block(VGet(26, 1, 8), VGet(2, 8, 15), BlockType::INVISIBLE_WALL)); // east wall (X:26..28)
        // Two fast spikes out of phase (speed=1.5 -> 2.67s cycle)
        blocks.push_back(Block(VGet(23, 1, 12), VGet(3, 1, 2), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.5f, 0.00f));
        blocks.push_back(Block(VGet(23, 1, 18), VGet(3, 1, 2), BlockType::RETRACTABLE_SPIKES, 0, false, VGet(0,0,0), VGet(0,0,0), 1.5f, 1.34f));

        // === FINISH ===
        // gap=2 in Z from the end of corridor 2 (Z=23), platform X:19..29, Z:25..33
        blocks.push_back(Block(VGet(19, 0, 25), VGet(10, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 1, 28), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 17: // Level 17 - "Moving maze": 5 platforms across 3 directions, a fork with a dead end
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-3, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));

        // === Section 1: Two moving platforms along +X, different phase ===
        // A (phase 0.0, Z=0): wait for it to arrive, then jump
        blocks.push_back(Block(VGet(5, 0, 0), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(5, 0, 0), VGet(19, 0, 0), 1.0f, 0.0f));
        // B (phase 0.5, Z=5): starts mid-path - different timing
        blocks.push_back(Block(VGet(5, 0, 5), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(5, 0, 5), VGet(19, 0, 5), 1.0f, 0.5f));

        // Intermediate landing (reachable from A or B at max X)
        // A/B max: X=19, right edge=22 -> landing at X:22..27
        blocks.push_back(Block(VGet(22, 0, -1), VGet(5, 1, 10), BlockType::PLATFORM));

        // === Section 2: The fork ===
        // C (DEAD END, +Z): Z:12->26, X=23 - no way off the island
        blocks.push_back(Block(VGet(23, 0, 12), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(23, 0, 12), VGet(23, 0, 26), 1.0f, 0.3f));
        blocks.push_back(Block(VGet(20, 0, 29), VGet(8, 1, 8), BlockType::PLATFORM)); // dead-end island

        // D (CORRECT, +X): X:28->44, Z=4 - leads onward
        // Gap from the landing (right edge X=27) to D's start (X=28) = 1 ✓
        blocks.push_back(Block(VGet(28, 0, 4), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(28, 0, 4), VGet(44, 0, 4), 1.0f, 0.0f));

        // Landing 2 (after D): D max X=44, right edge=48 -> landing at X:48..56
        blocks.push_back(Block(VGet(48, 0, 2), VGet(8, 1, 6), BlockType::PLATFORM));

        // === Section 3: Vertical platform ===
        // E (Y: 0->11): X=58, Z=4 - lifts the player to the upper tier
        // Gap from landing 2 (right X=56) to E's start (X=58) = 2 ✓
        // E is a side lift next to the upper platform: rises to Y=12, player jumps off to the right
        blocks.push_back(Block(VGet(58, 0, 4), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(58, 0, 4), VGet(58, 12, 4), 1.0f, 0.7f));

        // Upper platform Y=12: X=63 (gap=1 from the lift's right edge X=62) - jump off the lift here
        blocks.push_back(Block(VGet(63, 12, 2), VGet(8, 1, 8), BlockType::PLATFORM));

        // === Section 4: Two platforms moving toward each other ===
        // F (->, Z=2): X:73->89
        blocks.push_back(Block(VGet(73, 12, 2), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(73, 12, 2), VGet(89, 12, 2), 1.0f, 0.0f));
        // G (<-, Z=6): X:89->73 - runs opposite F
        blocks.push_back(Block(VGet(89, 12, 6), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(89, 12, 6), VGet(73, 12, 6), 1.0f, 0.0f));

        // Final platform (F/G max X=89, right edge=92 -> finish X:92..99)
        blocks.push_back(Block(VGet(92, 12, 3), VGet(7, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(94, 13, 5), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 18: // Level 18 - "Decoy buttons": the real ones open bridges, the fakes wall you in
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === Zone 1: One button - teaching the mechanic ===
        blocks.push_back(Block(VGet(5, 0, -2), VGet(9, 1, 6), BlockType::PLATFORM)); // X:5..14, Z:-2..4
        // Real button (linkId=1) - opens a bridge across the gap
        blocks.push_back(Block(VGet(11, 1, 1), VGet(1, 1, 1), BlockType::BUTTON, 1));
        // Bridge across the gap (linkId=1, starts inactive)
        blocks.push_back(Block(VGet(16, 0, -1), VGet(9, 1, 4), BlockType::DISAPPEARING, 1, false));
        // Platform past the bridge
        blocks.push_back(Block(VGet(27, 0, -2), VGet(8, 1, 7), BlockType::PLATFORM)); // X:27..35, Z:-2..5

        // === Zone 2: Two buttons - one real, one a decoy ===
        blocks.push_back(Block(VGet(37, 0, -3), VGet(12, 1, 9), BlockType::PLATFORM)); // X:37..49, Z:-3..6
        // Button A (DECOY, linkId=2) - raises an invisible wall right in the path to the bridge
        blocks.push_back(Block(VGet(40, 1, -1), VGet(1, 1, 1), BlockType::BUTTON, 2));
        blocks.push_back(Block(VGet(49, 0, -3), VGet(1, 6, 9), BlockType::INVISIBLE_WALL, 2, false));
        // Button B (REAL, linkId=3) - opens the bridge
        blocks.push_back(Block(VGet(44, 1, 3), VGet(1, 1, 1), BlockType::BUTTON, 3));
        // Bridge (linkId=3, starts inactive)
        blocks.push_back(Block(VGet(51, 0, -1), VGet(9, 1, 4), BlockType::DISAPPEARING, 3, false));
        // Platform past the second bridge
        blocks.push_back(Block(VGet(62, 0, -3), VGet(14, 1, 9), BlockType::PLATFORM)); // X:62..76, Z:-3..6

        // === Zone 3: Three buttons - two decoys, one real ===
        // Button L (DECOY, linkId=4) - wall blocks the exit from the platform (X=76)
        blocks.push_back(Block(VGet(65, 1, -1), VGet(1, 1, 1), BlockType::BUTTON, 4));
        blocks.push_back(Block(VGet(76, 0, -3), VGet(1, 6, 9), BlockType::INVISIBLE_WALL, 4, false));
        // Button M (REAL, linkId=5) - opens a bridge (X:77->86)
        blocks.push_back(Block(VGet(69, 1, 1), VGet(1, 1, 1), BlockType::BUTTON, 5));
        blocks.push_back(Block(VGet(77, 0, -1), VGet(9, 1, 4), BlockType::DISAPPEARING, 5, false));
        // Button R (DECOY, linkId=6) - wall in the middle of the bridge (X=81): you can step onto it, but not across
        blocks.push_back(Block(VGet(73, 1, 4), VGet(1, 1, 1), BlockType::BUTTON, 6));
        blocks.push_back(Block(VGet(81, 0, -3), VGet(1, 6, 9), BlockType::INVISIBLE_WALL, 6, false));

        // Final platform + trigger
        blocks.push_back(Block(VGet(88, 0, -2), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(90, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 19: // Level 19 - "Double bluff": 5 sections of escalating misdirection
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // === Section 1: FAKE bridge straight ahead + the real path via FAKE_SPIKES off to the side ===
        // Four FAKE_PLATFORM in a row - the player falls through on the very first step
        blocks.push_back(Block(VGet(5,  0, -1), VGet(4, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(11, 0, -1), VGet(4, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(17, 0, -1), VGet(4, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(23, 0, -1), VGet(4, 1, 3), BlockType::FAKE_PLATFORM));
        // The real path is off to the side (Z:5..9) with FAKE_SPIKES - looks scary, but it's safe
        // Gap from start (Z:-4..3) to the side path (Z=5): a 2-unit sideways jump ✓
        blocks.push_back(Block(VGet(5, 0, 5), VGet(24, 1, 5), BlockType::PLATFORM)); // X:5..29
        blocks.push_back(Block(VGet(9,  1, 6), VGet(3, 1, 3), BlockType::FAKE_SPIKES));
        blocks.push_back(Block(VGet(18, 1, 6), VGet(3, 1, 3), BlockType::FAKE_SPIKES));
        // Rejoin platform (back toward the center)
        blocks.push_back(Block(VGet(29, 0, 0), VGet(7, 1, 10), BlockType::PLATFORM));

        // === Section 2: FAKE_SPIKES x2 -> real SPIKES trap ===
        // Player gets used to pink meaning safe, then gets caught by red
        blocks.push_back(Block(VGet(36, 0, 0), VGet(22, 1, 8), BlockType::PLATFORM)); // X:36..58
        blocks.push_back(Block(VGet(38, 1, 1), VGet(3, 1, 5), BlockType::FAKE_SPIKES)); // pink (safe)
        blocks.push_back(Block(VGet(44, 1, 1), VGet(3, 1, 5), BlockType::FAKE_SPIKES)); // pink (safe)
        blocks.push_back(Block(VGet(50, 1, 1), VGet(3, 1, 4), BlockType::SPIKES));       // RED - death! (Z:1..5)
        // Way around at the far edge (Z:5..8) - 3 units of clear passage
        blocks.push_back(Block(VGet(55, 1, 1), VGet(3, 1, 5), BlockType::FAKE_SPIKES)); // safe again

        // === Section 3: CRUMBLING (yellow) + FAKE_PLATFORM (grey) - jump across the greys ===
        // Grey ones fall through, yellow ones crumble -> need to hop yellow to yellow
        // Size C=4, F=1, gap=1: jump distance C->C = 1gap + 1F + 1gap = 3 units ✓
        blocks.push_back(Block(VGet(60, 0, 2), VGet(4, 1, 4), BlockType::CRUMBLING));    // yellow (X:60..64)
        blocks.push_back(Block(VGet(65, 0, 2), VGet(1, 1, 4), BlockType::FAKE_PLATFORM));// grey, gap=1 (X:65..66)
        blocks.push_back(Block(VGet(67, 0, 2), VGet(4, 1, 4), BlockType::CRUMBLING));    // yellow, gap=1 (X:67..71)
        blocks.push_back(Block(VGet(72, 0, 2), VGet(1, 1, 4), BlockType::FAKE_PLATFORM));// grey (X:72..73)
        blocks.push_back(Block(VGet(74, 0, 2), VGet(4, 1, 4), BlockType::CRUMBLING));    // yellow (X:74..78)
        blocks.push_back(Block(VGet(81, 0, 1), VGet(7, 1, 6), BlockType::PLATFORM));     // landing (gap=3)

        // === Section 4: The obvious route is FAKE, the real path is a moving platform ===
        // FAKE_PLATFORM straight ahead (Z:1..6): looks like "there's the bridge" -> drops the player
        blocks.push_back(Block(VGet(91, 0, 1), VGet(9, 1, 5), BlockType::FAKE_PLATFORM));
        // Real MOVING platform (Z:7..10): reachable from the landing (Z:1..7) with gap=0 ✓
        blocks.push_back(Block(VGet(91, 0, 7), VGet(3, 1, 4), BlockType::MOVING,
            0, true, VGet(91, 0, 7), VGet(105, 0, 7), 1.0f, 0.0f));
        // Landing off the moving platform (X:107..115, Z:5..12)
        blocks.push_back(Block(VGet(107, 0, 5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === Section 5: Finale - three RETRACTABLE_SPIKES in a wave ===
        // Spikes span the full width (Z:3..11) -> timing is all that matters
        blocks.push_back(Block(VGet(117, 0, 3), VGet(16, 1, 9), BlockType::PLATFORM)); // X:117..133
        // Phases: cycleTime=2.67s -> offset by 0.89s between spikes (a proper wave)
        blocks.push_back(Block(VGet(119, 1, 3), VGet(2, 1, 9), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.0f));
        blocks.push_back(Block(VGet(124, 1, 3), VGet(2, 1, 9), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.89f));
        blocks.push_back(Block(VGet(129, 1, 3), VGet(2, 1, 9), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.5f, 1.78f));
        // Finish
        blocks.push_back(Block(VGet(135, 0, 3), VGet(7, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(137, 1, 5), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 20: // Level 20 - "Block 2 grand finale": every mechanic in one final gauntlet
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // === Section 1: CRUMBLING sprint - 5 platforms, no stopping ===
        // gap=2 between each, crumbles after 0.5s
        blocks.push_back(Block(VGet(5,  0, -1), VGet(4, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(11, 0, -1), VGet(4, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(17, 0, -1), VGet(4, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(23, 0, -1), VGet(4, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(29, 0, -1), VGet(4, 1, 4), BlockType::CRUMBLING));
        // Rest stop 1
        blocks.push_back(Block(VGet(35, 0, -2), VGet(7, 1, 6), BlockType::PLATFORM));

        // === Section 2: RETRACTABLE_SPIKES corridor - 4 spikes in a wave (speed=1.5) ===
        // cycleTime=2.67s: phases evenly offset by 0.67s
        blocks.push_back(Block(VGet(44, 0, -2), VGet(24, 1, 5), BlockType::PLATFORM)); // X:44..68
        blocks.push_back(Block(VGet(47, 1, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES,
            0, true,  VGet(0,0,0), VGet(0,0,0), 1.5f, 0.0f));
        blocks.push_back(Block(VGet(52, 1, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES,
            0, true,  VGet(0,0,0), VGet(0,0,0), 1.5f, 0.67f));
        blocks.push_back(Block(VGet(57, 1, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES,
            0, true,  VGet(0,0,0), VGet(0,0,0), 1.5f, 1.34f)); // 1.34 > halfCycle(1.333) -> reliably starts retracted
        blocks.push_back(Block(VGet(62, 1, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES,
            0, true,  VGet(0,0,0), VGet(0,0,0), 1.5f, 2.0f));
        // Rest stop 2
        blocks.push_back(Block(VGet(70, 0, -2), VGet(7, 1, 6), BlockType::PLATFORM));

        // === Section 3: MOVING x2 - two moving platforms with a landing in between ===
        // A (slow, phase=0.0): X:79->94, available right away
        blocks.push_back(Block(VGet(79, 0, 0), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(79, 0, 0), VGet(94, 0, 0), 1.0f, 0.0f));
        // Intermediate landing (A max=X:94..98 -> landing X:97..103, gap=1 ✓)
        blocks.push_back(Block(VGet(97, 0, -1), VGet(6, 1, 6), BlockType::PLATFORM));
        // B (faster, phase=0.4): X:105->122, starts closer to mid-path -> needs a short wait
        blocks.push_back(Block(VGet(105, 0, 1), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(105, 0, 1), VGet(122, 0, 1), 1.0f, 0.4f));
        // Landing (B max=X:122..126 -> landing X:124..132, gap=1 ✓)
        blocks.push_back(Block(VGet(124, 0, -1), VGet(8, 1, 7), BlockType::PLATFORM));

        // === Section 4: FAKE_SPIKES x2 -> SPIKES trap + FAKE_PLATFORM over a chasm ===
        blocks.push_back(Block(VGet(134, 0, -1), VGet(20, 1, 8), BlockType::PLATFORM)); // X:134..154
        blocks.push_back(Block(VGet(137, 1,  0), VGet(3, 1, 6), BlockType::FAKE_SPIKES));  // pink (safe)
        blocks.push_back(Block(VGet(143, 1,  0), VGet(3, 1, 6), BlockType::FAKE_SPIKES));  // pink (safe)
        blocks.push_back(Block(VGet(149, 1,  0), VGet(3, 1, 5), BlockType::SPIKES));        // RED - death! (Z:0..5)
        // Way around at the far edge (Z:5..7) - 2 units of clear passage
        // FAKE_PLATFORM across the chasm (straight ahead, Z:-1..5): drops the player
        blocks.push_back(Block(VGet(156, 0, -1), VGet(8, 1, 6), BlockType::FAKE_PLATFORM));
        // Real landing (Z:6..12): requires a sideways jump from the edge of section 4
        blocks.push_back(Block(VGet(156, 0, 6), VGet(8, 1, 7), BlockType::PLATFORM));

        // === Section 5: BUTTON - decoy (-> invisible wall) + real (-> bridge) ===
        blocks.push_back(Block(VGet(166, 0, 6), VGet(12, 1, 8), BlockType::PLATFORM)); // X:166..178
        // Decoy button (linkId=7): wall blocks the exit from the platform (X=178)
        blocks.push_back(Block(VGet(168, 1, 8),  VGet(1, 1, 1), BlockType::BUTTON, 7));
        blocks.push_back(Block(VGet(178, 0, 6),  VGet(1, 6, 8), BlockType::INVISIBLE_WALL, 7, false));
        // Real button (linkId=8): opens a bridge across the gap
        blocks.push_back(Block(VGet(174, 1, 10), VGet(1, 1, 1), BlockType::BUTTON, 8));
        blocks.push_back(Block(VGet(180, 0, 8),  VGet(10, 1, 4), BlockType::DISAPPEARING, 8, false));
        // Landing past the bridge
        blocks.push_back(Block(VGet(192, 0, 7), VGet(8, 1, 7), BlockType::PLATFORM));

        // === Section 6: MOVING + CRUMBLING combo - the final push ===
        // Moving platform carries the player to the CRUMBLING chain
        blocks.push_back(Block(VGet(202, 0, 9), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(202, 0, 9), VGet(218, 0, 9), 1.0f, 0.0f));
        // Three CRUMBLING platforms - no stopping (gap=2)
        // MOVING max: X:218..222 -> first CRUMBLING at X=221, gap=1 ✓
        blocks.push_back(Block(VGet(221, 0, 8), VGet(4, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(227, 0, 8), VGet(4, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(233, 0, 8), VGet(4, 1, 4), BlockType::CRUMBLING));
        // Final platform + trigger
        blocks.push_back(Block(VGet(239, 0, 7), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(241, 1, 9), VGet(2, 1, 2), BlockType::TRIGGER));
        break;
    }
}
