# Architecture

~7,100 lines of C++ across 28 files, no engine — just DxLib for windowing/rendering/audio/input. Everything else (physics, state machines, save format, localization) is hand-rolled.

## Directory layout

```
MindTrapa/
├── main.cpp                  — WinMain, game loop, top-level state dispatch
├── level.h / level.cpp       — Block/Level: collision, block state, per-block rendering
├── level_block1..5.cpp       — level data (block lists), one file per themed block
├── LevelManager.h/.cpp       — current level, death count, timer, HUD text
├── Player.h/.cpp             — physics, input, animation state machine
├── Camera.h/.cpp             — orbit camera (mouse look + follow)
├── Menu.h/.cpp                — stack-based UI state machine, all screens
├── ModelManager.h/.cpp       — singleton: loads/duplicates DxLib models
├── AudioManager.h/.cpp       — BGM streaming + SFX playback
├── ProfileManager.h/.cpp     — singleton: 5 save slots, settings, theme packs
├── Localization.h/.cpp       — singleton: EN/RU/JA strings + per-language fonts
├── Renderer.h/.cpp            — skybox (sphere/procedural), fog, starfield, planets
├── ParticleSystem.h/.cpp     — lightweight particle effects
├── Constants.h                — shared constants (screen size, etc.)
├── tools/                     — ModelConverter: custom OBJ→DirectX .x converter
├── models/, media/, fonts/    — assets (only files actually referenced are tracked in git)
└── saves/                     — runtime save data (gitignored)
```

## Core loop

`main.cpp` owns one big state machine (`GameState`: `MAIN_MENU`, `LEVEL_SELECT_BLOCKS/LEVELS`, `SETTINGS`, `PROFILE_SELECT/CREATE`, `PLAYING`, `PAUSED`, `DYING`, `LEVEL_RESULT`, `MUSIC_SELECT`) driving what gets updated/drawn each frame. Per-frame order during gameplay matters and was a real source of bugs:

1. `Camera::UpdateAngles()` — mouse look, **before** player physics, so movement direction uses this frame's look direction
2. `Player::Update()` — reads input, applies gravity/collision, updates animation state
3. Teleport check (repositions the player if triggered)
4. `Camera::Apply()` — positions/aims the camera at the *current*-frame player position, **after** physics/teleport

Splitting the camera update this way fixed a one-frame lag between the camera and the rendered player model that was most visible during fast falls (the camera used to be updated before `Player::Update()`, so it always followed last frame's position).

## Class map

| Class | Responsibility |
|---|---|
| `Level` | Block list for one level: collision (`CheckCollision`, AABB, smallest-overlap-axis resolution), triggers (win/death/teleport), per-block draw (model or primitive fallback) |
| `LevelManager` | Owns the current `Level`, tracks death count / timer, exposes best-time lookups |
| `Player` | Physics (gravity, jump, ice inertia), collision response, animation state machine (Idle/Run/Jump/Fall/Death/Wave/Yes), facing direction |
| `Camera` | Third-person orbit camera; angle update and position/target application are separate calls (see above) |
| `Menu` | All UI screens as one stack-based state machine (`PushState`/back-navigation), slider/button widgets, settings live-apply |
| `ModelManager` | Singleton; loads a `.x`/`.mv1` once per `ModelID`, duplicates the DxLib model handle per block instance; explicit `BlockType → ModelID` switch (not a positional cast — two enums coincidentally shared a value early on, this was a real bug) |
| `AudioManager` | BGM: one streamed handle per block per theme pack, hot-reloadable; SFX: pre-loaded short clips (jump/land/death/win/bounce/teleport) |
| `ProfileManager` | Singleton; 5 profile slots, each a plain INI-style text file (`saves/slot_N.dat`) with stars/best-times/settings/theme-pack; sequential level-unlock logic (level *N* unlocked iff level *N-1* has stars > 0) |
| `Localization` | Singleton; `StrId` enum indexes parallel string tables for EN/RU/JA, plus a separate font handle per language (Cyrillic/Kana need different font files than the UI's Orbitron) |
| `Renderer` | Spherical skybox (equirectangular texture, falls back to a procedural starfield+planet if no texture is found), per-biome fog, two-layer twinkling starfield |

## Level data model

Levels are **not** data-driven — there's no JSON/level-format parser. Each level is a C++ function body (`LoadBlock1()`…`LoadBlock5()`, dispatched by ID range) that pushes `Block` structs directly into a `std::vector`. `Block` carries a `BlockType`, position/size, and a grab-bag of mechanic-specific fields (`linkId`, `moveTarget`, `gravityMultiplier`, `swingAngle`, `frictionMultiplier`, …) — one struct covers all 17 mechanics rather than a type hierarchy. This is a deliberate simplicity trade-off: fast to iterate on for a solo dev authoring 50 levels by hand, at the cost of some fields only being meaningful for a subset of block types.

## Model/asset pipeline

Downloaded asset packs (Quaternius/Kenney/KayKit, all CC0) aren't used directly — `tools/ModelConverter` converts OBJ meshes to DxLib's native `.x` format. Only the specific converted files actually referenced by `ModelManager::LoadModel()` calls in `main.cpp` are kept in the repo (the original packs are multiple hundreds of MB; the game itself references a few dozen files totaling a few MB).

## Save system

Plain text, not binary — easy to inspect/hand-edit for testing. `saves/slot_N.dat` (`NAME=`, `STARS=` comma-list of 50, `BEST_TIMES=` comma-list of 50, per-profile settings) plus `saves/config.dat` (last-used slot, UI language). Theme packs unlock at a total-star threshold (75⭐/150⭐) computed from `Profile::TotalStars()`.

## Rendering notes

- Skybox tries a per-biome JPG/PNG texture first, falls back to a fully procedural starfield+planet if the texture is missing — the game never hard-fails on missing skybox assets
- Darkness (Block 4) is a full-screen overlay driven by a single float that rises while moving and decays at rest, not a per-block lighting system
- F3 toggles a debug overlay: every block type gets a distinct wireframe color, useful for verifying level geometry without needing the real models loaded
