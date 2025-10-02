# MindTrapa - 3D Trolling Puzzle Platformer

A 3D puzzle platformer inspired by levelDEVIL, designed to troll and challenge players with creative mechanics and unexpected twists.

## 🎮 Game Concept

100 levels divided into 10 blocks (10 levels each), where each block introduces new trolling mechanics:
- Block 1 (Levels 1-10): Basic jumps and movement
- Block 2 (Levels 11-20): Deadly spikes and traps
- Block 3 (Levels 21-30): Buttons and mechanisms
- Block 4 (Levels 31-40): Teleportation chaos
- Block 5 (Levels 41-50): Inverted controls
- ...and more!

## 🛠️ Tech Stack

- **Language**: C++
- **Graphics Library**: DxLib (DirectX wrapper)
- **Platform**: Windows
- **IDE**: Visual Studio

## 🎯 Current Status

**Version**: Alpha 0.1  
**Levels**: 6/100 implemented  
**Mechanics**: 9 block types implemented

### Working Features:
- ✅ 3D platformer movement (WASD + mouse camera)
- ✅ Jump mechanics (Space)
- ✅ Collision system (all axes)
- ✅ Death counter and level restart (R)
- ✅ Spikes (instant death)
- ✅ Invisible walls
- ✅ Teleports (portal mechanics)
- ✅ Buttons and doors (E to activate)
- ✅ Level progression system

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
    PLATFORM,         // Normal solid platform
    TRIGGER,          // Level completion trigger
    INVISIBLE_WALL,   // Invisible but solid wall
    FAKE_PLATFORM,    // Looks solid but you fall through
    SPIKES,           // Instant death
    DISAPPEARING,     // Platform that appears/disappears
    MOVING,           // Moving platform (future)
    BUTTON,           // Activatable button (E key)
    TELEPORT          // Portal to another location
};
```

## 📋 TODO

See [ROADMAP.md](ROADMAP.md) for detailed development plan.

Short-term:
- [ ] Add levels 7-10 (complete first block)
- [ ] Implement disappearing platforms with timer
- [ ] Add moving platforms
- [ ] Create simple menu

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