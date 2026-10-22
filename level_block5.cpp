#include "Level.h"

void Level::LoadBlock5(int id) {
    switch (id) {

    case 41: // Level 41 - ICE: Z-shaped zigzag — brake before turns
        // Three ice strips with two 90-degree turns.
        // Carry too much speed into a turn and you fly straight off the edge.
        // Ice is slicker than usual (0.992). Only way off it is the BOUNCE_PAD at the end.
        playerSpawn = VGet(0, 1, 0);

        // Starting platform
        blocks.push_back(Block(VGet(-4, -1, -2), VGet(9, 1, 6), BlockType::PLATFORM));

        // Strip 1 (+X direction): X:5..23, Z:0..4, width 4
        { Block b(VGet(5, -1, 0), VGet(18, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.992f; blocks.push_back(b); }
        // Turn 1 (X:21..29, Z:-2..7): brake along X, build up Z
        { Block b(VGet(21, -1, -2), VGet(8, 1, 9), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.992f; blocks.push_back(b); }
        // Strip 2 (+Z direction): X:22..26, Z:4..22, width 4
        { Block b(VGet(22, -1, 4), VGet(4, 1, 18), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.992f; blocks.push_back(b); }
        // Turn 2 (X:19..29, Z:20..28): brake along Z, build up X
        { Block b(VGet(19, -1, 20), VGet(10, 1, 8), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.992f; blocks.push_back(b); }
        // Strip 3 (+X direction): X:27..52, Z:22..26, width 4
        { Block b(VGet(27, -1, 22), VGet(25, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.992f; blocks.push_back(b); }

        // BOUNCE_PAD at the end of strip 3 — the only way up
        blocks.push_back(Block(VGet(52, -1, 23), VGet(4, 1, 3), BlockType::BOUNCE_PAD));

        // Final platform (top=7): reachable via bounce pad + ice momentum
        blocks.push_back(Block(VGet(58, 6, 21), VGet(9, 1, 7), BlockType::PLATFORM));
        // Trigger sits right on the platform surface — no pedestal
        blocks.push_back(Block(VGet(60, 7, 23), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 42: // Level 42 - ICE + gates: Z-zigzag with alternating gate positions
        // The whole level is one continuous ice sheet, no dry-land breaks.
        // 7 spike gates with alternating gaps — you have to keep drifting on the ice.
        // frictionMultiplier=0.9936 (20% slicker than level 41), only way out is BOUNCE_PAD.
        playerSpawn = VGet(0, 1, 2); // Z=2, centered on the first strip

        // Starting platform (Z:-1..6)
        blocks.push_back(Block(VGet(-4, -1, -1), VGet(8, 1, 7), BlockType::PLATFORM));

        // Strip 1 (+X, X:4..22, Z:0..6, width 6)
        { Block b(VGet(4, -1, 0), VGet(18, 1, 6), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.9936f; blocks.push_back(b); }
        // Gate 1 (X=9): gap Z=3..6, spikes Z=0..3
        blocks.push_back(Block(VGet(9, 0, 0), VGet(2, 1, 3), BlockType::SPIKES));
        // Gate 2 (X=16): gap Z=0..3, spikes Z=3..6 (offset)
        blocks.push_back(Block(VGet(16, 0, 3), VGet(2, 1, 3), BlockType::SPIKES));

        // Turn 1 (X:20..28, Z:-2..9) — wide angle to give room to maneuver
        { Block b(VGet(20, -1, -2), VGet(8, 1, 11), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.9936f; blocks.push_back(b); }

        // Strip 2 (+Z, X:21..27, Z:6..24, width 6)
        { Block b(VGet(21, -1, 6), VGet(6, 1, 18), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.9936f; blocks.push_back(b); }
        // Gate 3 (Z=11): gap X=24..27, spikes X=21..24
        blocks.push_back(Block(VGet(21, 0, 11), VGet(3, 1, 2), BlockType::SPIKES));
        // Gate 4 (Z=17): gap X=21..24, spikes X=24..27 (offset)
        blocks.push_back(Block(VGet(24, 0, 17), VGet(3, 1, 2), BlockType::SPIKES));

        // Turn 2 (X:18..29, Z:22..30) — connects to the third strip
        { Block b(VGet(18, -1, 22), VGet(11, 1, 8), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.9936f; blocks.push_back(b); }

        // Strip 3 (+X, X:26..52, Z:23..29, width 6)
        { Block b(VGet(26, -1, 23), VGet(26, 1, 6), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.9936f; blocks.push_back(b); }
        // Gate 5 (X=32): gap Z=26..29, spikes Z=23..26
        blocks.push_back(Block(VGet(32, 0, 23), VGet(2, 1, 3), BlockType::SPIKES));
        // Gate 6 (X=40): gap Z=23..26, spikes Z=26..29 (offset)
        blocks.push_back(Block(VGet(40, 0, 26), VGet(2, 1, 3), BlockType::SPIKES));
        // Gate 7 (X=47): tight — gap Z=24..27, spikes on both sides
        blocks.push_back(Block(VGet(47, 0, 23), VGet(2, 1, 1), BlockType::SPIKES)); // Z:23..24
        blocks.push_back(Block(VGet(47, 0, 27), VGet(2, 1, 2), BlockType::SPIKES)); // Z:27..29

        // BOUNCE_PAD at the end of strip 3
        blocks.push_back(Block(VGet(52, -1, 25), VGet(4, 1, 3), BlockType::BOUNCE_PAD));

        // Final platform (top=7)
        blocks.push_back(Block(VGet(58, 6, 23), VGet(9, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(60, 7, 25), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 43: // Level 43 - BOUNCE: mid-air forks + fake platforms
        // Fork 1: decoy bounce pad ONLY on the fake (Z+); take the real one (Z-) and jump to fork 2
        // Fork 2: ground level, both paths same height, fake straight ahead, real is Z+
        // Fork 3: top=6, plain one=fake, pink spikes=real
        // Pads: starting pad sits 1 unit below the platform (you walk off the edge and fall onto it)
        //       the rest sit at pos.y = platform_top (flush with the platform, no overlap)
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-4, -1, -3), VGet(8, 1, 7), BlockType::PLATFORM));
        // Bounce pad 1: 1 unit below the start (top=-1) — walk off the edge and drop onto it
        blocks.push_back(Block(VGet(4, -2, 0), VGet(3, 1, 3), BlockType::BOUNCE_PAD));

        // === FORK 1: X:18..25 ===
        // Fake (Z+): decoy bounce pad on top (top=1) — you fall through the fake, the pad launches you
        blocks.push_back(Block(VGet(18, -1,  3), VGet(7, 1, 4), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(21,  0,  4), VGet(3, 1, 3), BlockType::BOUNCE_PAD)); // pos.y=platform_top=0
        // Real (Z-): no bounce pad — jump from here to fork 2
        blocks.push_back(Block(VGet(18, -1, -6), VGet(7, 1, 4), BlockType::PLATFORM));

        // === FORK 2: X:30..37, ground level, top=0 — jump here from fork 1's real path ===
        // Fake (straight ahead, Z:-5..-1)
        blocks.push_back(Block(VGet(30, -1, -5), VGet(7, 1, 4), BlockType::FAKE_PLATFORM));
        // Real (Z+, Z:2..6) — requires a sideways jump
        blocks.push_back(Block(VGet(30, -1,  2), VGet(7, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(33,  0,  3), VGet(3, 1, 3), BlockType::BOUNCE_PAD)); // pos.y=platform_top=0

        // === FORK 3: X:44..51, top=6 ===
        // Fake (plain, straight ahead, Z:2..6)
        blocks.push_back(Block(VGet(44, 5,  2), VGet(7, 1, 4), BlockType::FAKE_PLATFORM));
        // Real (with pink spikes, Z:9..13)
        blocks.push_back(Block(VGet(44, 5,  9), VGet(7, 1, 4), BlockType::PLATFORM));
        for (int tx = 44; tx < 51; tx++)
            for (int tz = 9; tz < 13; tz++)
                blocks.push_back(Block(VGet((float)tx, 6.f, (float)tz), VGet(1, 1, 1), BlockType::FAKE_SPIKES));
        blocks.push_back(Block(VGet(47,  6, 10), VGet(3, 1, 3), BlockType::BOUNCE_PAD)); // pos.y=platform_top=6

        // Final platform (top=12)
        blocks.push_back(Block(VGet(58, 11,  8), VGet(9, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(60, 12,  9), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 44: // Level 44 — ICE + BOUNCE: three strips in three directions, mini spike mazes
        // +X (top=0) -> bounce -> +Z (top=7) -> bounce -> +X (top=14) -> trigger
        // Each strip has corner spikes and zigzag obstacles laid right on the ice
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-3, -1, -2), VGet(7, 1, 5), BlockType::PLATFORM));

        // === STRIP 1: +X, top=0, width Z:-2..3 ===
        { Block b(VGet(4, -1, -2), VGet(18, 1, 5), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        // Corner spikes at the entrance (decorative, clip the corners)
        blocks.push_back(Block(VGet(4, 0, -2), VGet(2, 1, 1), BlockType::SPIKES)); // entrance, left corner
        blocks.push_back(Block(VGet(4, 0,  2), VGet(2, 1, 1), BlockType::SPIKES)); // entrance, right corner
        // Zigzag on the ice: right -> left -> right
        blocks.push_back(Block(VGet( 9, 0,  1), VGet(3, 1, 2), BlockType::SPIKES)); // Z:1..3, gap on the left
        blocks.push_back(Block(VGet(15, 0, -2), VGet(3, 1, 3), BlockType::SPIKES)); // Z:-2..1, gap on the right
        blocks.push_back(Block(VGet(20, 0,  1), VGet(2, 1, 2), BlockType::SPIKES)); // Z:1..3, near the exit
        // Bounce pad (rolls you off the ice)
        blocks.push_back(Block(VGet(22, -1, -1), VGet(3, 1, 4), BlockType::BOUNCE_PAD));

        // === STRIP 2: +Z, top=7, width X:33..42 ===
        { Block b(VGet(33, 6, 2), VGet(9, 1, 16), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        // Corner spikes at the landing zone (X-corners at Z=2)
        blocks.push_back(Block(VGet(33, 7,  2), VGet(2, 1, 3), BlockType::SPIKES)); // landing, left corner
        blocks.push_back(Block(VGet(40, 7,  2), VGet(2, 1, 3), BlockType::SPIKES)); // landing, right corner
        // Zigzag on the ice: right (X+) -> left (X-)
        blocks.push_back(Block(VGet(33, 7,  8), VGet(4, 1, 3), BlockType::SPIKES)); // X:33..37, gap on the right
        blocks.push_back(Block(VGet(39, 7, 13), VGet(3, 1, 3), BlockType::SPIKES)); // X:39..42, gap on the left
        // Bounce pad (rolls you off along Z)
        blocks.push_back(Block(VGet(34, 6, 18), VGet(7, 1, 3), BlockType::BOUNCE_PAD));

        // === STRIP 3: +X, top=14, width Z:27..39 ===
        { Block b(VGet(33, 13, 27), VGet(20, 1, 12), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        // Corner spikes at the landing zone (Z-corners at X=33)
        blocks.push_back(Block(VGet(33, 14, 27), VGet(3, 1, 2), BlockType::SPIKES)); // lower corner
        blocks.push_back(Block(VGet(33, 14, 37), VGet(3, 1, 2), BlockType::SPIKES)); // upper corner
        // Zigzag on the ice: forward along Z+ then back along Z-
        blocks.push_back(Block(VGet(39, 14, 34), VGet(4, 1, 5), BlockType::SPIKES)); // Z:34..39, gap below
        blocks.push_back(Block(VGet(45, 14, 27), VGet(4, 1, 5), BlockType::SPIKES)); // Z:27..32, gap above
        // Trigger
        blocks.push_back(Block(VGet(50, 14, 30), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 45: // Level 45 — ICE + MOVING: S-shaped route with 3 gaps bridged by moving platforms
        // +X -> turn -> +Z -> turn -> -X -> finish
        // Every strip has a 6-unit gap — no crossing it without the shuttle platform.
        // Key mechanic: ice brakes poorly, so you need to judge your entry speed onto each platform.
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-4, -1, -3), VGet(8, 1, 7), BlockType::PLATFORM));

        // === STRIP 1: +X, Z:-2..2 (width 4) ===
        // Ice A: X:4..14
        { Block b(VGet(4, -1, -2), VGet(10, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        // GAP 1 (X:14..20): platform shuttles between ice A and ice B
        { Block m(VGet(14, -1, -2), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(14, -1, -2), VGet(20, -1, -2), 1.2f, 0.0f);
          blocks.push_back(m); }
        // Ice B: X:20..30, spike corridor
        { Block b(VGet(20, -1, -2), VGet(10, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(23, 0,  1), VGet(3, 1, 1), BlockType::SPIKES)); // right wall
        blocks.push_back(Block(VGet(27, 0, -2), VGet(2, 1, 1), BlockType::SPIKES)); // left wall

        // Turn 1 (ice corner X:28..36, Z:-2..8)
        { Block b(VGet(28, -1, -2), VGet(8, 1, 10), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }

        // === STRIP 2: +Z, X:32..36 (width 4) ===
        // Ice A: Z:6..16
        { Block b(VGet(32, -1, 6), VGet(4, 1, 10), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(32, 0, 10), VGet(1, 1, 3), BlockType::SPIKES)); // left wall
        // GAP 2 (Z:16..22): platform shuttles along Z
        { Block m(VGet(32, -1, 16), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(32, -1, 16), VGet(32, -1, 22), 1.0f, 0.0f);
          blocks.push_back(m); }
        // Ice B: Z:22..34, spike corridor
        { Block b(VGet(32, -1, 22), VGet(4, 1, 12), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(35, 0, 26), VGet(1, 1, 3), BlockType::SPIKES)); // right wall
        blocks.push_back(Block(VGet(32, 0, 32), VGet(2, 1, 2), BlockType::SPIKES)); // corner near the turn

        // Turn 2 (ice corner X:24..36, Z:32..40)
        { Block b(VGet(24, -1, 32), VGet(12, 1, 8), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }

        // === STRIP 3: -X, Z:36..40 (width 4) ===
        // Ice A: X:12..26 (entered from turn 2)
        { Block b(VGet(12, -1, 36), VGet(14, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(17, 0, 39), VGet(4, 1, 1), BlockType::SPIKES)); // top wall
        // GAP 3 (X:6..12): platform starts near ice B, shuttles toward ice A
        { Block m(VGet(6, -1, 36), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(6, -1, 36), VGet(8, -1, 36), 1.1f, 0.0f);
          blocks.push_back(m); }
        // Ice B: X:0..6
        { Block b(VGet(0, -1, 36), VGet(6, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(1, 0, 36), VGet(2, 1, 1), BlockType::SPIKES)); // bottom corner

        // Final platform
        blocks.push_back(Block(VGet(-8, -1, 35), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(-6, 0, 37), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 46: // Level 46 — ICE + CRUMBLING: pick up speed on the ice and fly into a crumbling chain
        // Chain 1: 5 crumbling blocks, Z±1 zigzag, forgiving — this is where you learn the rhythm
        // Chain 2: 3 crumbling blocks, Z±2 zigzag, narrower — harder to aim right after the ice
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-4, -1, -3), VGet(8, 1, 7), BlockType::PLATFORM));

        // Ice 1 (build-up): X:4..18, Z:-2..2 — spikes funnel you into the center corridor
        { Block b(VGet(4, -1, -2), VGet(14, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(8,  0,  1), VGet(5, 1, 1), BlockType::SPIKES)); // top wall
        blocks.push_back(Block(VGet(13, 0, -2), VGet(3, 1, 1), BlockType::SPIKES)); // bottom wall

        // === CHAIN 1: 5 crumbling blocks, gap 3, Z±1 zigzag ===
        // You fly straight off the ice onto C1 — every gap between platforms needs a jump
        blocks.push_back(Block(VGet(18, -1, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:-1..2
        blocks.push_back(Block(VGet(24, -1,  0), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:0..3
        blocks.push_back(Block(VGet(30, -1, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:-1..2
        blocks.push_back(Block(VGet(36, -1,  0), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:0..3
        blocks.push_back(Block(VGet(42, -1, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:-1..2

        // Ice again instead of a resting platform — no breather before chain 2
        { Block b(VGet(45, -1, -2), VGet(11, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(50, 0,  1), VGet(3, 1, 1), BlockType::SPIKES)); // top wall
        blocks.push_back(Block(VGet(53, 0, -2), VGet(2, 1, 1), BlockType::SPIKES)); // bottom wall

        // === CHAIN 2: 3 crumbling blocks, gap 3, Z±2 zigzag — 1-unit overlap ===
        // Straight off the ice onto C6, no stop — less time to line up your aim
        blocks.push_back(Block(VGet(56, -1, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:-1..2
        blocks.push_back(Block(VGet(62, -1,  1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:1..4, shift+2
        blocks.push_back(Block(VGet(68, -1, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:-1..2, shift-2

        // Finish
        blocks.push_back(Block(VGet(71, -1, -2), VGet(8, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(73, 0, -1), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 47: // Level 47 — three gravity zones: low-g -> normal -> high-g
        // Low-g (0.65x): jumps ~8 units — floaty but controllable, 4 small floating platforms
        // Normal: feels heavy by comparison during the crossing, short bridge
        // High-g (2.5x): jumps ~2 units — only the bounce pad saves you, and barely
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-4, -1, -3), VGet(8, 1, 7), BlockType::PLATFORM));

        // === ZONE 1: LOW-G (0.65x) — X:4..36 ===
        // Jump reaches ~8 units — floaty, but you won't overshoot. No bounce pad, needs precision.
        { Block g(VGet(4, -3, -6), VGet(32, 30, 14), BlockType::GRAVITY_ZONE);
          g.gravityMultiplier = 0.65f; blocks.push_back(g); }
        // Chain of 3 floating platforms (4x1x6), Z±4 zigzag, +5 in Y each time
        // Blinking spikes run along Z — they block the X-edge of the entry/exit, phases offset
        // A (top=5): Z:-1..5 — spike on the right X-edge (X:12..13), don't overshoot into it
        blocks.push_back(Block(VGet(9, 4, -1), VGet(4, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(12, 5, -1), VGet(1, 1, 6), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.0f));  // X:12..13, phase 0
        // B (top=10): shifted Z-4 -> Z:-5..1 — spike on the left X-edge (X:17..18), needs a hop over
        blocks.push_back(Block(VGet(17, 9, -5), VGet(4, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17, 10, -5), VGet(1, 1, 6), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.4f)); // X:17..18, phase 0.4
        // C (top=15): shifted Z+4 -> Z:-1..5 — spike on the right X-edge (X:27..28)
        blocks.push_back(Block(VGet(24, 14, -1), VGet(4, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(27, 15, -1), VGet(1, 1, 6), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.8f)); // X:27..28, phase 0.8

        // === BRIDGE: normal gravity — shortened to X:30..36 ===
        // Gap X:36..41 — the jump lands right on the zone boundary, gravity switches mid-air
        blocks.push_back(Block(VGet(30, 17, -2), VGet(6, 1, 5), BlockType::PLATFORM)); // X:30..36

        // === ZONE 3: HIGH-G (2.5x) — X:42..80 ===
        // Jump reaches ~2 units, bounce pad ~5 units — every ascent is razor-thin
        { Block g(VGet(42, -3, -4), VGet(38, 38, 11), BlockType::GRAVITY_ZONE);
          g.gravityMultiplier = 2.5f; blocks.push_back(g); }
        // Platform D (top=18): starts at X:41 — catches the jump across the gap
        blocks.push_back(Block(VGet(41, 17, -2), VGet(9, 1, 5), BlockType::PLATFORM));
        // Bounce pad 1 on D (top=19): launches to 19+4.8=23.8 — just reaches E (top=23)
        blocks.push_back(Block(VGet(44, 18, -1), VGet(3, 1, 3), BlockType::BOUNCE_PAD));
        // Platform E (top=23)
        blocks.push_back(Block(VGet(50, 22, -2), VGet(7, 1, 5), BlockType::PLATFORM));
        // Bounce pad 2 on E (top=24): launches to 24+4.8=28.8 — just reaches F (top=28)
        blocks.push_back(Block(VGet(52, 23, -1), VGet(3, 1, 3), BlockType::BOUNCE_PAD));
        // Final platform F (top=28)
        blocks.push_back(Block(VGet(58, 27, -2), VGet(8, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(60, 28, -1), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 48: // Level 48 — ICE + darkness: 5 turns, only one flash and it lands on T3
        // Darkness builds up instantly (2f/s). One mid-route flash — memorize the route while you can.
        // Finish: 3 real steps lead up to a bounce pad to the goal. Everything ahead of them is fake.
        // Corridor E: fake spikes instead of real ones — a scare for nothing.
        playerSpawn = VGet(0, 1, 2);

        // Start
        blocks.push_back(Block(VGet(-4, -1, -2), VGet(8, 1, 7), BlockType::PLATFORM));

        // No light zones here — the level is dark the entire time (DARKEN_SPEED=2f/s)

        // --- A: +X, Z:0..4 ---
        { Block b(VGet(4, -1, 0), VGet(12, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(7,  0, 3), VGet(4, 1, 1), BlockType::SPIKES)); // far wall
        blocks.push_back(Block(VGet(12, 0, 0), VGet(3, 1, 1), BlockType::SPIKES)); // near wall

        // --- T1: +X -> +Z ---
        { Block b(VGet(14, -1, -2), VGet(8, 1, 10), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(14, 0, -2), VGet(2, 1, 3), BlockType::SPIKES)); // inner corner

        // --- B: +Z, X:20..24 ---
        { Block b(VGet(20, -1, 6), VGet(4, 1, 14), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(20, 0, 10), VGet(1, 1, 4), BlockType::SPIKES)); // far wall
        blocks.push_back(Block(VGet(20, 0, 17), VGet(1, 1, 2), BlockType::SPIKES)); // end of B

        // --- T2: +Z -> +X ---
        { Block b(VGet(16, -1, 18), VGet(10, 1, 10), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(16, 0, 18), VGet(3, 1, 2), BlockType::SPIKES)); // inner corner

        // --- C: +X, Z:24..28 ---
        { Block b(VGet(24, -1, 24), VGet(14, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(27, 0, 27), VGet(4, 1, 1), BlockType::SPIKES)); // far wall
        blocks.push_back(Block(VGet(33, 0, 24), VGet(3, 1, 1), BlockType::SPIKES)); // near wall

        // --- T3: +X -> +Z (the flash lands around this corner) ---
        { Block b(VGet(36, -1, 22), VGet(10, 1, 12), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(36, 0, 22), VGet(3, 1, 2), BlockType::SPIKES)); // inner corner

        // --- D: +Z, X:42..46 ---
        { Block b(VGet(42, -1, 32), VGet(4, 1, 14), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(45, 0, 36), VGet(1, 1, 5), BlockType::SPIKES)); // far wall
        blocks.push_back(Block(VGet(42, 0, 43), VGet(1, 1, 2), BlockType::SPIKES)); // near wall

        // --- T4: +Z -> +X ---
        { Block b(VGet(38, -1, 44), VGet(12, 1, 10), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(38, 0, 44), VGet(3, 1, 2), BlockType::SPIKES)); // inner corner

        // --- E: +X, Z:52..56 (FAKE_SPIKES — look scary, harmless) ---
        { Block b(VGet(48, -1, 52), VGet(14, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(51, 0, 55), VGet(6, 1, 1), BlockType::FAKE_SPIKES)); // FAKE — scare tactic
        blocks.push_back(Block(VGet(52, 0, 52), VGet(3, 1, 1), BlockType::SPIKES));      // the real threat

        // --- T5: +X -> +Z ---
        { Block b(VGet(60, -1, 50), VGet(10, 1, 14), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(60, 0, 50), VGet(2, 1, 3), BlockType::SPIKES)); // inner corner

        // --- F: +Z, X:66..70 ---
        { Block b(VGet(66, -1, 62), VGet(4, 1, 14), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(69, 0, 66), VGet(1, 1, 5), BlockType::SPIKES)); // far wall
        blocks.push_back(Block(VGet(69, 0, 73), VGet(1, 1, 2), BlockType::SPIKES)); // end of F

        // BOUNCE_PAD — the way off the ice
        blocks.push_back(Block(VGet(66, -1, 75), VGet(4, 1, 4), BlockType::BOUNCE_PAD));

        // === FINAL SECTION: fork in the middle ===
        // Landing pad (top=7) after the bounce off the pad
        blocks.push_back(Block(VGet(64, 6, 79), VGet(10, 1, 8), BlockType::PLATFORM));

        // Step A — real (4x4)
        blocks.push_back(Block(VGet(65, 6, 89), VGet(4, 1, 4), BlockType::PLATFORM));

        // Step B — real (3x3) with fake spikes — a scare for nothing
        blocks.push_back(Block(VGet(66, 6, 96), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(66, 7, 96), VGet(3, 1, 2), BlockType::FAKE_SPIKES));

        // *** FORK (top=7, 8x6): step on it and see two paths ***
        blocks.push_back(Block(VGet(64, 6, 102), VGet(8, 1, 6), BlockType::PLATFORM));

        // Fake path (+Z) — three steps ahead, look like the way forward
        blocks.push_back(Block(VGet(65, 6, 111), VGet(4, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(65, 6, 117), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(65, 6, 123), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));

        // Real path (+X from the fork) — one small real platform with the trigger
        blocks.push_back(Block(VGet(72, 6, 103), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(72, 7, 103), VGet(4, 1, 4), BlockType::TRIGGER));
        break;

    case 49: // Level 49 — ICE + RETRACTABLE_SPIKES + CRUMBLING
        // Three ice corridors with a slalom of blinking gates (5-unit corridors).
        // Between sections — crumbling bridges with 5-6 unit gaps: pick up speed and don't stall.
        playerSpawn = VGet(0, 1, 2);

        // Start
        blocks.push_back(Block(VGet(-3, -1, -1), VGet(7, 1, 7), BlockType::PLATFORM));

        // === SECTION A: +X, corridor Z:0..5, 4 slalom gates ===
        { Block b(VGet(4, -1, 0), VGet(16, 1, 5), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(7,  0, 0), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.4f, 0.0f));   // G1: left, Z:0..2
        blocks.push_back(Block(VGet(11, 0, 3), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.4f, 0.35f));  // G2: right, Z:3..5
        blocks.push_back(Block(VGet(14, 0, 0), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.0f));   // G3: left
        blocks.push_back(Block(VGet(17, 0, 3), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.6f));   // G4: right

        // === CRUMBLING BRIDGE 1 (X:20..44) ===
        blocks.push_back(Block(VGet(20, -1, 0), VGet(4, 1, 5), BlockType::CRUMBLING, 0, true)); // C1, full width
        // Gap X:24..30 (6 units)
        blocks.push_back(Block(VGet(30, -1, 1), VGet(4, 1, 3), BlockType::CRUMBLING, 0, true)); // C2, narrow Z:1..4
        // Gap X:34..40 (6 units)
        blocks.push_back(Block(VGet(40, -1, 0), VGet(4, 1, 5), BlockType::CRUMBLING, 0, true)); // C3, full width

        // Corner platform (transition to section B)
        blocks.push_back(Block(VGet(44, -1, -2), VGet(9, 1, 9), BlockType::PLATFORM));

        // === SECTION B: +Z, corridor X:47..52, 4 gates ===
        { Block b(VGet(47, -1, 5), VGet(5, 1, 17), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(47, 0, 9),  VGet(2, 1, 1), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.3f, 0.0f));   // G5: X-left (X:47..49)
        blocks.push_back(Block(VGet(50, 0, 14), VGet(2, 1, 1), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.3f, 0.43f));  // G6: X-right (X:50..52)
        blocks.push_back(Block(VGet(47, 0, 18), VGet(2, 1, 1), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.1f, 0.0f));   // G7: X-left
        blocks.push_back(Block(VGet(50, 0, 19), VGet(2, 1, 1), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.1f, 0.55f));  // G8: X-right

        // === CRUMBLING BRIDGE 2 (Z:22..44) ===
        blocks.push_back(Block(VGet(47, -1, 22), VGet(5, 1, 4), BlockType::CRUMBLING, 0, true)); // D1, full width
        // Gap Z:26..31 (5 units)
        blocks.push_back(Block(VGet(48, -1, 31), VGet(3, 1, 4), BlockType::CRUMBLING, 0, true)); // D2, narrow X:48..51
        // Gap Z:35..40 (5 units)
        blocks.push_back(Block(VGet(47, -1, 40), VGet(5, 1, 4), BlockType::CRUMBLING, 0, true)); // D3, full width

        // Corner platform (transition to section C)
        blocks.push_back(Block(VGet(44, -1, 44), VGet(9, 1, 8), BlockType::PLATFORM));

        // === SECTION C: +X, corridor Z:46..51, 5 gates (normal gravity) ===
        // Spike rhythm speeds up: 1.0s -> 0.9s -> 0.8s.
        { Block b(VGet(53, -1, 46), VGet(21, 1, 5), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(57, 0, 46), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));   // G9:  left
        blocks.push_back(Block(VGet(61, 0, 49), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.25f));  // G10: right
        blocks.push_back(Block(VGet(65, 0, 46), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.9f, 0.0f));   // G11: left
        blocks.push_back(Block(VGet(69, 0, 49), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.9f, 0.45f));  // G12: right
        blocks.push_back(Block(VGet(73, 0, 46), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.8f, 0.0f));   // G13: left (fastest one)

        // BOUNCE_PAD — the way off the ice (normal gravity here)
        blocks.push_back(Block(VGet(74, -1, 47), VGet(3, 1, 3), BlockType::BOUNCE_PAD));

        // Final platform (top=7) + trigger
        blocks.push_back(Block(VGet(79, 6, 45), VGet(9, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(81, 7, 47), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 50: // Level 50 — FINALE: every mechanic
        // S1: fork (fakes + moving platform) -> S2: crumbling blocks under blinking spikes ->
        // S3: low-g + blades -> S4: darkness + disappearing platforms -> S5: ice + bounce pad
        playerSpawn = VGet(0, 1, 3);

        // Start
        blocks.push_back(Block(VGet(-4, -1, -2), VGet(8, 1, 10), BlockType::PLATFORM));

        // ================================================================
        // SECTION 1: FORK — three paths, two fake, one real
        // ================================================================
        // Moving platform bridges the gap X:4..10
        { Block m(VGet(4, -1, 0), VGet(6, 1, 6), BlockType::MOVING,
              0, true, VGet(4,-1,0), VGet(6,-1,0), 1.0f, 0.0f);
          blocks.push_back(m); }

        // Fork hub: three corridors along Z
        blocks.push_back(Block(VGet(10, -1, -4), VGet(8, 1, 14), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17,  0, -4), VGet(1, 1, 2), BlockType::FAKE_SPIKES)); // corner by path A
        blocks.push_back(Block(VGet(17,  0,  8), VGet(1, 1, 2), BlockType::FAKE_SPIKES)); // corner by path C

        // Path A (Z:-4..0): FAKE — clean, wide, looks "safe"
        blocks.push_back(Block(VGet(18, -1, -4), VGet(10, 1, 4), BlockType::FAKE_PLATFORM));

        // Path B (Z:1..5): REAL — blinking spikes, wait for the opening
        blocks.push_back(Block(VGet(18, -1,  1), VGet(10, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(21,  0,  1), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.0f));
        blocks.push_back(Block(VGet(25,  0,  3), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.5f));

        // Path C (Z:6..10): FAKE — with fake spikes (looks dangerous = "must be the real one")
        blocks.push_back(Block(VGet(18, -1,  6), VGet(10, 1, 4), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(20,  0,  7), VGet(4, 1, 2), BlockType::FAKE_SPIKES));

        // Landing after the fork (only path B, Z:1..5, actually reaches here)
        blocks.push_back(Block(VGet(28, -1,  0), VGet(7, 1, 6), BlockType::PLATFORM));

        // ================================================================
        // SECTION 2: CRUMBLING BLOCKS + BLINKING SPIKES ON EACH ONE
        // ================================================================
        blocks.push_back(Block(VGet(35, -1,  0), VGet(4, 1, 4), BlockType::CRUMBLING, 0, true)); // R1
        blocks.push_back(Block(VGet(36,  0,  1), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.0f));
        // Gap X:39..44
        blocks.push_back(Block(VGet(44, -1,  1), VGet(4, 1, 3), BlockType::CRUMBLING, 0, true)); // R2
        blocks.push_back(Block(VGet(45,  0,  1), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.1f, 0.3f));
        // Gap X:48..53
        blocks.push_back(Block(VGet(53, -1,  0), VGet(4, 1, 4), BlockType::CRUMBLING, 0, true)); // R3
        blocks.push_back(Block(VGet(54,  0,  1), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        // Gap X:57..62
        blocks.push_back(Block(VGet(62, -1,  0), VGet(4, 1, 4), BlockType::CRUMBLING, 0, true)); // R4
        blocks.push_back(Block(VGet(63,  0,  1), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.9f, 0.45f));

        blocks.push_back(Block(VGet(66, -1, -1), VGet(7, 1, 7), BlockType::PLATFORM));

        // ================================================================
        // SECTION 3: LOW-G (0.65x) + BLADES OVER THE GAPS
        // ================================================================
        // All platforms sit at ground level. Jumps go 1.5x farther — but the blades guard the gaps.
        { Block gz(VGet(73, -3, -5), VGet(38, 20, 12), BlockType::GRAVITY_ZONE);
          gz.gravityMultiplier = 0.65f; blocks.push_back(gz); }

        blocks.push_back(Block(VGet(73, -1, -2), VGet(6, 1, 5), BlockType::PLATFORM)); // GA
        // Gap X:79..84, blade centered in the gap
        { Block p(VGet(79, 2,  0), VGet(2, 1, 2), BlockType::PENDULUM_BLADE,
              0, true, VGet(0,0,0), VGet(0,0,0), 1.4f, 0.0f);
          p.pivotPoint = VGet(80, 8, 1);
          p.swingSpeed = 1.3f;
          p.swingRange = DX_PI_F / 2.8f;
          blocks.push_back(p); }
        blocks.push_back(Block(VGet(84, -1,  1), VGet(6, 1, 5), BlockType::PLATFORM)); // GB (Z shift)
        // Gap X:90..96, blade swings faster
        { Block p(VGet(90, 2, -1), VGet(2, 1, 2), BlockType::PENDULUM_BLADE,
              0, true, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.5f);
          p.pivotPoint = VGet(91, 8, 0);
          p.swingSpeed = 1.4f;
          p.swingRange = DX_PI_F / 2.5f;
          blocks.push_back(p); }
        blocks.push_back(Block(VGet(96, -1, -2), VGet(6, 1, 5), BlockType::PLATFORM)); // GC
        // Gap X:102..107 — no blade here, this is the exit from the zone (zone spans X:73..111)
        blocks.push_back(Block(VGet(107, -1, -2), VGet(7, 1, 6), BlockType::PLATFORM));

        // ================================================================
        // SECTION 4: DARKNESS (LIGHT_PULSE_ZONE) + DISAPPEARING PLATFORMS
        // ================================================================
        // Darkness at 2f/s, flash every 2s. Platforms alternate phase — one's there while the other isn't.
        blocks.push_back(Block(VGet(114, -3, -4), VGet(32, 20, 11), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 2.0f, 0.0f));

        blocks.push_back(Block(VGet(114, -1, -1), VGet(6, 1, 5), BlockType::DISAPPEARING,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.0f));  // E1
        // Gap X:120..125
        blocks.push_back(Block(VGet(125, -1, -1), VGet(6, 1, 5), BlockType::DISAPPEARING,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.5f));  // E2, opposite phase
        // Gap X:131..136
        blocks.push_back(Block(VGet(136, -1, -1), VGet(6, 1, 5), BlockType::DISAPPEARING,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.0f));  // E3

        blocks.push_back(Block(VGet(142, -1, -2), VGet(7, 1, 7), BlockType::PLATFORM));

        // ================================================================
        // SECTION 5: ICE + RETRACTABLE GATES + BOUNCE PAD TO THE FINISH
        // ================================================================
        { Block b(VGet(149, -1, -1), VGet(22, 1, 5), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        // 5 gates: left/right along Z, rhythm 1.0s->0.8s
        blocks.push_back(Block(VGet(152, 0, -1), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));   // left
        blocks.push_back(Block(VGet(156, 0,  2), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.25f));  // right
        blocks.push_back(Block(VGet(160, 0, -1), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.9f, 0.0f));   // left
        blocks.push_back(Block(VGet(164, 0,  2), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.9f, 0.45f));  // right
        blocks.push_back(Block(VGet(168, 0, -1), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.8f, 0.0f));   // left (fastest one)

        // BOUNCE_PAD — the way off the ice to the finish
        blocks.push_back(Block(VGet(171, -1,  0), VGet(3, 1, 3), BlockType::BOUNCE_PAD));

        // Final platform (top=10) — reachable via the bounce pad (max height ~12u)
        blocks.push_back(Block(VGet(177,  9, -1), VGet(10, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(179, 10,  1), VGet(4, 1, 4), BlockType::TRIGGER));
        break;
    }
}
