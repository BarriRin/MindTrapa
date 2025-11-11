# MindTrapa - Contribution Guide

> 3D Trolling Puzzle Platformer inspired by levelDEVIL

---

## 🎮 Project Overview

**Genre:** 3D Puzzle Platformer (trolling/challenge)
**Tech Stack:** C++ + DxLib (DirectX wrapper)
**Platform:** Windows
**Goal:** 100 levels across 10 thematic blocks

---

## 🛠️ Building the Project

### Prerequisites
- Visual Studio 2019 or later
- DxLib library ([download here](https://dxlib.xsrv.jp/))
- Windows SDK

### Setup
1. Clone the repository
2. Install DxLib and configure include/lib paths in VS project settings
3. Open `MindTrapa.sln`
4. Build (F5 or Ctrl+Shift+B)

### Project Configuration
- Target: x86 (DxLib requirement)
- DxLib include paths must be configured
- Debug/Release configurations available

---

## 📐 Code Style

### C++ Conventions
- **Classes:** PascalCase (`LevelManager`, `BlockType`)
- **Functions:** PascalCase (`CheckCollision()`, `Update()`)
- **Variables:** camelCase (`playerPos`, `isActive`, `moveSpeed`)
- **Constants:** UPPER_CASE or const variables

### Patterns
- Use `enum class` for type safety
- RAII principles for resource management
- Header/implementation split (.h/.cpp)
- Clear separation: rendering, logic, input handling

---

## 🏗️ Architecture Basics

### Core Systems
- **main.cpp:** Game loop, input handling, state management
- **Level.cpp:** Individual level logic, collision, block mechanics
- **LevelManager.cpp:** Level creation and transitions
- **Menu.cpp:** UI and level selection

### Coordinate System
- **Y-up:** Y=0 is ground level
- **3D AABB collision** on all axes

### Block Types
```cpp
enum class BlockType {
    PLATFORM,           // Normal solid platform
    TRIGGER,            // Level completion trigger
    SPIKES,             // Instant death
    RETRACTABLE_SPIKES, // Timed spikes (2s active/2s hidden)
    CRUMBLING,          // Falls 0.5s after touch, respawns
    FAKE_SPIKES,        // Trolling - looks deadly but harmless (pink)
    INVISIBLE_WALL,     // Solid but invisible
    FAKE_PLATFORM,      // Looks solid but passthrough
    TELEPORT,           // Portal mechanics
    BUTTON,             // Activates linked objects
    DISAPPEARING,       // Button-controlled or timed
    MOVING              // Oscillates between two points
};
```

---

## 🎯 Contributing

### Adding a New Level
1. Edit `LevelManager::CreateLevel(int id)`
2. Define platforms using Block structure:
   ```cpp
   {pos, size, BlockType, linkId, isActive, moveStart, moveTarget, moveSpeed, timer}
   ```
3. Set player spawn position
4. Test collision and mechanics
5. Update level count in Menu if needed

### Adding a New Mechanic
1. Add to `BlockType` enum in `Level.h`
2. Implement rendering in `Level::Render()`
3. Implement behavior in `Level::Update()` or collision system
4. Update `MECHANICS.md` documentation

### Pull Requests
- Clear description of changes
- Test your changes thoroughly
- Follow existing code style
- Update documentation if needed

---

## 🎮 Controls

- **WASD:** Movement (camera-relative)
- **Mouse:** Camera rotation
- **Space:** Jump
- **E:** Activate button
- **R:** Restart level
- **[ / ]:** Decrease/increase game speed (for testing)
- **ESC:** Return to menu

---

## 📚 Documentation

- **README.md:** Project overview and status
- **ROADMAP.md:** Development plan and phases
- **MECHANICS.md:** Detailed mechanic explanations
- **ARCHITECTURE.md:** System architecture (if available)

---

## 🐛 Known Issues

See GitHub issues for bug reports and feature requests.

---

## 📄 License

Personal project - all rights reserved.

---

## 🙏 Thanks

Built with DxLib library by Yamada Takumi (やまだ たくみ)

---

*For development notes and session history, maintainers should check `.claude/*.local.md` files (not tracked in git)*
