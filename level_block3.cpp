#include "Level.h"

void Level::LoadBlock3(int id) {
    switch (id) {
    case 21: // Level 21 - Introducing LOW GRAVITY
        playerSpawn = VGet(0, 1, 0);

        // Start platform
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Regular platform before the zone
        blocks.push_back(Block(VGet(5, 0, -2), VGet(4, 1, 4), BlockType::PLATFORM));

        // Low-gravity zone (floaty jumps)
        blocks.push_back(Block(VGet(11, 0, -3), VGet(15, 15, 6), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.3f;

        // Platforms inside the low-gravity zone
        blocks.push_back(Block(VGet(12, 1, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17, 4, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 7, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Exit from the zone and finish
        blocks.push_back(Block(VGet(28, 8, -2), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(30, 9, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 22: // Level 22 - Introducing HIGH GRAVITY
        playerSpawn = VGet(0, 1, 0);

        // Start platform
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // High-gravity zone (heavy, fast fall)
        blocks.push_back(Block(VGet(5, -1, -4), VGet(20, 12, 8), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 2.5f;

        // Platforms inside the zone - jumps are low
        blocks.push_back(Block(VGet(6, 0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(10, 0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(14, 0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(18, 0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 1, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Finish
        blocks.push_back(Block(VGet(27, 2, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(29, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 23: // Level 23 - Introducing PENDULUM BLADE (perpendicular to the path)
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === Corridor 1: one slow pendulum ===
        // Pendulum swings perpendicular to travel (+X): size.z=4 spans the full width Z:-2..2
        // Can't be skirted around the side - timing is the only way through
        blocks.push_back(Block(VGet(5, 0, -2), VGet(17, 1, 4), BlockType::PLATFORM)); // X:5..22
        blocks.push_back(Block(VGet(11, 1, -2), VGet(1.5f, 1.5f, 4.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(11, 8, -2); // pivot.z = pos.z (Z stays fixed while swinging)
        blocks.back().swingSpeed = 1.2f;
        blocks.back().swingRange = DX_PI_F / 4.0f;

        // Rest platform
        blocks.push_back(Block(VGet(22, 1, -3), VGet(6, 1, 6), BlockType::PLATFORM));

        // === Corridor 2: a faster pendulum ===
        blocks.push_back(Block(VGet(30, 1, -2), VGet(18, 1, 4), BlockType::PLATFORM)); // X:30..48
        blocks.push_back(Block(VGet(38, 2, -2), VGet(1.5f, 1.5f, 4.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(38, 9, -2);
        blocks.back().swingSpeed = 2.0f;
        blocks.back().swingRange = DX_PI_F / 3.5f;

        // Finish
        blocks.push_back(Block(VGet(50, 2, -3), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(52, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 24: // Level 24 - COMBO: Low Gravity + Pendulum (perpendicular to the path)
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Extended low-gravity zone (X:5..43, Z:-3..3)
        blocks.push_back(Block(VGet(5, 0, -3), VGet(38, 22, 6), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.4f;

        // P1
        blocks.push_back(Block(VGet(6, 1, -2), VGet(4, 1, 4), BlockType::PLATFORM));
        // P2
        blocks.push_back(Block(VGet(12, 5, -2), VGet(4, 1, 4), BlockType::PLATFORM)); // top surface Y=6

        // === Pendulum 1 - perpendicular to the path, size.z=6 spans the full width Z:-3..3 ===
        // Swings in the XY plane, can't be skirted around the side - timing only
        blocks.push_back(Block(VGet(17, 3, -3), VGet(1.5f, 1.5f, 6.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(17, 12, -3); // pivot.z = pos.z (Z stays fixed while swinging)
        blocks.back().swingSpeed = 1.5f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        // P3
        blocks.push_back(Block(VGet(20, 8, -2), VGet(4, 1, 4), BlockType::PLATFORM)); // top surface Y=9
        // P4
        blocks.push_back(Block(VGet(26, 11, -2), VGet(4, 1, 4), BlockType::PLATFORM)); // top surface Y=12

        // === Pendulum 2 - faster, phase-shifted ===
        blocks.push_back(Block(VGet(31, 9, -3), VGet(1.5f, 1.5f, 6.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(31, 17, -3);
        blocks.back().swingSpeed = 1.8f;
        blocks.back().swingRange = DX_PI_F / 3.5f;
        blocks.back().timer = 1.5f; // phase offset

        // P5
        blocks.push_back(Block(VGet(34, 13, -2), VGet(4, 1, 4), BlockType::PLATFORM)); // top surface Y=14
        // P6
        blocks.push_back(Block(VGet(40, 15, -2), VGet(4, 1, 4), BlockType::PLATFORM)); // top surface Y=16

        // Finish (past the gravity zone)
        blocks.push_back(Block(VGet(45, 16, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(47, 17, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 25: // Level 25 - REVERSE GRAVITY!
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Platform before the zone
        blocks.push_back(Block(VGet(5, 0, -2), VGet(4, 1, 4), BlockType::PLATFORM));

        // REVERSE-gravity zone
        blocks.push_back(Block(VGet(11, 0, -3), VGet(18, 18, 6), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = -1.2f;

        // The "ceiling" is now the floor - platforms up top
        blocks.push_back(Block(VGet(12, 12, -2), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17, 12, -2), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(23, 12, -2), VGet(4, 1, 4), BlockType::PLATFORM));

        // Exit from the zone - landing platform
        blocks.push_back(Block(VGet(30, 0, -3), VGet(4, 1, 6), BlockType::PLATFORM));

        // Jump across the gap to the finish (normal gravity applies again)
        blocks.push_back(Block(VGet(37, 0, -3), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(40, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 26: // Level 26 - COMPLEX PENDULUM PATTERNS (4 pendulums, perpendicular to the path)
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Long corridor with four pendulums (Z:-2..2, width 4)
        blocks.push_back(Block(VGet(5, 0, -2), VGet(45, 1, 4), BlockType::PLATFORM));

        // Pendulum 1 - slow
        // size.z=4 spans the full corridor width Z:-2..2, no way around
        blocks.push_back(Block(VGet(10, 1, -2), VGet(1.5f, 1.5f, 4.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(10, 9, -2); // pivot.z = pos.z
        blocks.back().swingSpeed = 1.0f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        // Pendulum 2 - fast, phase-shifted
        blocks.push_back(Block(VGet(19, 1, -2), VGet(1.5f, 1.5f, 4.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(19, 10, -2);
        blocks.back().swingSpeed = 2.2f;
        blocks.back().swingRange = DX_PI_F / 2.5f;
        blocks.back().timer = 1.5f;

        // Pendulum 3 - medium, wide swing
        blocks.push_back(Block(VGet(29, 1, -2), VGet(2.0f, 2.0f, 4.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(29, 11, -2);
        blocks.back().swingSpeed = 1.6f;
        blocks.back().swingRange = DX_PI_F / 2.2f;
        blocks.back().timer = 3.0f;

        // Pendulum 4 - fastest, out-of-sync phase
        blocks.push_back(Block(VGet(39, 1, -2), VGet(1.5f, 1.5f, 4.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(39, 9, -2);
        blocks.back().swingSpeed = 2.5f;
        blocks.back().swingRange = DX_PI_F / 2.8f;
        blocks.back().timer = 0.8f;

        // Finish
        blocks.push_back(Block(VGet(52, 1, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(54, 2, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 27: // Level 27 - MULTIPLE GRAVITY ZONES + PENDULUMS
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Section 1: Low gravity + pendulum
        blocks.push_back(Block(VGet(5, 0, -2), VGet(12, 12, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.35f;

        blocks.push_back(Block(VGet(6, 1, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(10, 1, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(10, 10, -1);
        blocks.back().swingSpeed = 1.4f;
        blocks.back().swingRange = DX_PI_F / 3.5f;

        blocks.push_back(Block(VGet(13, 6, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // Section 2: High gravity + fast pendulum
        blocks.push_back(Block(VGet(18, -1, -2), VGet(12, 8, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 2.8f;

        blocks.push_back(Block(VGet(19, 0, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(23, 1, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(23, 6, -1);
        blocks.back().swingSpeed = 2.5f;
        blocks.back().swingRange = DX_PI_F / 4.0f;
        blocks.back().timer = 1.0f;

        blocks.push_back(Block(VGet(27, 1, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // Finish
        blocks.push_back(Block(VGet(32, 2, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(34, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 28: // Level 28 - GRAVITY TROLLING (unexpected switches)
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Platform 1
        blocks.push_back(Block(VGet(5, 0, -1), VGet(4, 1, 2), BlockType::PLATFORM));

        // TROLL: unexpected high gravity
        blocks.push_back(Block(VGet(11, 0, -2), VGet(8, 6, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 3.5f;

        // The real path - off to the side, with a low platform
        blocks.push_back(Block(VGet(12, -1, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(16, -1, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // Platform 2
        blocks.push_back(Block(VGet(21, 0, -2), VGet(4, 1, 4), BlockType::PLATFORM));

        // TROLL 2: looks like low gravity, but it's REVERSE!
        blocks.push_back(Block(VGet(27, 0, -2), VGet(10, 12, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = -1.5f;

        // Platforms up top
        blocks.push_back(Block(VGet(29, 10, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(33, 10, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // Landing platform after zone 2
        blocks.push_back(Block(VGet(39, 0, -3), VGet(6, 1, 6), BlockType::PLATFORM)); // X:39..45

        // TROLL 3: suddenly high gravity - jumps become very short
        blocks.push_back(Block(VGet(45, 0, -2), VGet(12, 8, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 3.0f;

        blocks.push_back(Block(VGet(46, 0, -1), VGet(2, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(50, 0, -1), VGet(2, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(54, 0, -1), VGet(2, 1, 2), BlockType::PLATFORM));

        // Finish
        blocks.push_back(Block(VGet(59, 0, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(61, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 29: // Level 29 - GAUNTLET OF BLADES + GRAVITY (hard!)
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Low-gravity zone spanning the whole level
        blocks.push_back(Block(VGet(5, 0, -3), VGet(45, 20, 6), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.4f;

        // Series of platforms with pendulums
        blocks.push_back(Block(VGet(6, 1, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Pendulums perpendicular to the path: size.z=6 spans the full zone width Z:-3..3
        blocks.push_back(Block(VGet(11, 2, -3), VGet(1.5f, 1.5f, 6.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(11, 12, -3); // pivot.z = pos.z
        blocks.back().swingSpeed = 1.8f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        blocks.push_back(Block(VGet(14, 4, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(19, 5, -3), VGet(1.5f, 1.5f, 6.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(19, 14, -3);
        blocks.back().swingSpeed = 2.0f;
        blocks.back().swingRange = DX_PI_F / 2.5f;
        blocks.back().timer = 1.2f;

        blocks.push_back(Block(VGet(22, 7, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(27, 8, -3), VGet(2.0f, 2.0f, 6.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(27, 16, -3);
        blocks.back().swingSpeed = 1.3f;
        blocks.back().swingRange = DX_PI_F / 2.0f;
        blocks.back().timer = 2.5f;

        blocks.push_back(Block(VGet(30, 10, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(35, 11, -3), VGet(1.5f, 1.5f, 6.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(35, 18, -3);
        blocks.back().swingSpeed = 2.4f;
        blocks.back().swingRange = DX_PI_F / 3.5f;

        // Finish
        blocks.push_back(Block(VGet(38, 12, -2), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(40, 13, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 30: // Level 30 - BLOCK 3 FINALE: EVERYTHING AT ONCE!
        playerSpawn = VGet(0, 1, 0);

        // Start
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === SECTION 1: Low gravity + pendulum ===
        blocks.push_back(Block(VGet(5, 0, -2), VGet(15, 15, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.35f;

        blocks.push_back(Block(VGet(6, 1, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(11, 3, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(11, 12, -1);
        blocks.back().swingSpeed = 1.6f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        blocks.push_back(Block(VGet(15, 7, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // === SECTION 2: High gravity + CRUMBLING ===
        blocks.push_back(Block(VGet(20, 0, -2), VGet(18, 10, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 2.6f;

        blocks.push_back(Block(VGet(21, 1, -1), VGet(3, 1, 2), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(25, 1, -1), VGet(3, 1, 2), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(29, 1, -1), VGet(3, 1, 2), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(33, 2, -1), VGet(4, 1, 2), BlockType::PLATFORM));

        // === SECTION 3: Reverse gravity + pendulums + RETRACTABLE_SPIKES ===
        blocks.push_back(Block(VGet(39, 0, -2), VGet(22, 16, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = -1.3f;

        // Ceiling platforms
        blocks.push_back(Block(VGet(40, 14, -1), VGet(4, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(46, 14, -1), VGet(4, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(52, 14, -1), VGet(4, 1, 2), BlockType::PLATFORM));

        // Pendulums on the "floor" (now the ceiling)
        blocks.push_back(Block(VGet(43, 8, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(43, 2, -1);
        blocks.back().swingSpeed = 1.7f;
        blocks.back().swingRange = DX_PI_F / 3.5f;

        blocks.push_back(Block(VGet(49, 7, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(49, 1, -1);
        blocks.back().swingSpeed = 2.1f;
        blocks.back().swingRange = DX_PI_F / 2.8f;
        blocks.back().timer = 1.5f;

        // Retractable spikes on the ceiling platforms
        blocks.push_back(Block(VGet(47, 15, -1), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES));

        // === SECTION 4: Final stretch - normal gravity + everything at once ===
        blocks.push_back(Block(VGet(63, 0, -3), VGet(6, 1, 6), BlockType::PLATFORM));

        // Moving platform
        blocks.push_back(Block(VGet(71, 2, -1), VGet(4, 1, 3), BlockType::MOVING, 0, true,
            VGet(71, 2, -1), VGet(71, 8, -1), 1.8f));

        // Final pendulum
        blocks.push_back(Block(VGet(77, 5, -1), VGet(1.5f, 1.5f, 3.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(77, 14, -1);
        blocks.back().swingSpeed = 2.0f;
        blocks.back().swingRange = DX_PI_F / 2.5f;

        // FINISH
        blocks.push_back(Block(VGet(80, 9, -3), VGet(7, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(83, 10, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;
    }
}
