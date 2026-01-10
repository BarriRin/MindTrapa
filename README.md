# MindTrapa - 3D Trolling Puzzle Platformer

A 3D puzzle platformer inspired by levelDEVIL, designed to troll and challenge players with creative mechanics and unexpected twists.

## 🎮 Game Concept

50 levels divided into 5 blocks (10 levels each), where each block introduces new trolling mechanics:
- **Block 1** (Levels 1-10): **Basics** - Movement, jumps, teleports ✅ COMPLETE
- **Block 2** (Levels 11-20): **Timing & Precision** - Retractable spikes, crumbling platforms ✅ COMPLETE
- **Block 3** (Levels 21-30): **Gravity & Blades** - Gravity zones, pendulum blades ⏳ IN PROGRESS
- **Block 4** (Levels 31-40): **TBD** - To be designed
- **Block 5** (Levels 41-50): **Final Challenge** - Combines all mechanics

## 🛠️ Tech Stack

- **Language**: C++
- **Graphics Library**: DxLib (DirectX wrapper)
- **Platform**: Windows
- **IDE**: Visual Studio

## 🎯 Current Status

**Version**: Alpha 0.3
**Levels**: 20/50 implemented (40% complete! 🎉)
**Mechanics**: 12 block types implemented

### Working Features:
- ✅ 3D platformer movement (WASD + mouse camera)
- ✅ Jump mechanics (Space)
- ✅ Collision system (all axes)
- ✅ Death counter and level restart (R)
- ✅ Multiple spike types (static, retractable, fake)
- ✅ Invisible walls
- ✅ Teleports (portal mechanics)
- ✅ Buttons and doors (E to activate)
- ✅ Moving platforms with player physics
- ✅ Crumbling platforms (fall after touch)
- ✅ Disappearing platforms (timed)
- ✅ Level progression system with timer
- ✅ Main menu and pause system
- ✅ ModelManager for 3D models/skyboxes (ready for assets)

## 🚀 How to Build

1. Install DxLib library
2. Open `MindTrapa.sln` in Visual Studio
3. Configure DxLib paths in project settings
4. Build and run

## 🎮 Controls

- **WASD** - Movement (relative to camera)
- **Mouse** - Camera rotation
- **Space** - Jump
- **E** - Activate button/switch
- **R** - Restart level
- **ESC** - Quit game

## 📁 Project Structure

```
MindTrapa/
├── Level.h / Level.cpp           - Level class (loading, rendering, collision)
├── LevelManager.h / LevelManager.cpp - Level management (transitions, deaths)
├── main.cpp                      - Game loop, input, rendering
├── ARCHITECTURE.md               - Architecture documentation
├── MECHANICS.md                  - Implemented mechanics list
└── ROADMAP.md                    - Development roadmap
```

## 🎨 Block Types

```cpp
enum class BlockType {
    PLATFORM,            // Normal solid platform
    TRIGGER,             // Level completion trigger
    INVISIBLE_WALL,      // Invisible but solid wall
    FAKE_PLATFORM,       // Looks solid but you fall through
    SPIKES,              // Instant death (static or timed)
    DISAPPEARING,        // Platform that appears/disappears
    MOVING,              // Moving platform
    BUTTON,              // Activatable button (E key)
    TELEPORT,            // Portal to another location
    RETRACTABLE_SPIKES,  // Spikes that retract on timer (Block 2)
    CRUMBLING,           // Falls after player touches (Block 2)
    FAKE_SPIKES,         // Looks deadly but harmless (trolling!)
    GRAVITY_ZONE,        // Changes gravity (Block 3) - COMING SOON
    PENDULUM_BLADE       // Swinging blade trap (Block 3) - COMING SOON
};
```

## 📋 TODO

See [ROADMAP.md](ROADMAP.md) for detailed development plan.

**Current Focus** (Block 3):
- [ ] Implement gravity zones (low/high/reverse gravity)
- [ ] Implement pendulum blade mechanics
- [ ] Create levels 21-30
- [ ] Find/create Jupiter-themed skybox

**Next Steps**:
- [ ] Block 4 design (levels 31-40)
- [ ] Block 5 final challenge (levels 41-50)
- [ ] Add 3D models and skyboxes
- [ ] Sound effects and music

## 🤝 Contributing

This is a personal learning project, but feedback and suggestions are welcome!

## 📄 License

Personal project - all rights reserved

## 🎓 Learning Resources

This project was developed as a learning exercise in:
- C++ game development
- 3D collision detection
- Game architecture patterns
- DxLib library usage