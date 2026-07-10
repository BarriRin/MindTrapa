# MindTrapa

A 3D puzzle-platformer inspired by *levelDEVIL* — levels that look simple but punish trust in the obvious. 50 hand-designed levels across 5 themed space blocks, built from scratch in C++ on top of DxLib, with no commercial game engine.

**Status:** 1.0 — feature-complete, all 50 levels playable, solo-developed by Ivan Danisheuski.

## Concept

The core trick: appearances lie. A clean-looking platform may not be solid. Spikes that look lethal may be harmless. A wall you can't see may still stop you. Every level teaches the player to verify before committing — then breaks that trust again in a new way in the next block.

## Tech Stack

- **Language:** C++ (Visual Studio 2022)
- **Graphics:** [DxLib](https://dxlib.xyz/) 3.24f — a DirectX wrapper, no engine (Unity/Unreal) involved
- **Platform:** Windows 10/11, built as Win32
- **Everything hand-rolled:** physics/collision, camera, animation state machine, audio (streamed BGM + SFX), localization, save/profile system, menu navigation, particle effects, skybox/fog rendering

See [ARCHITECTURE.md](ARCHITECTURE.md) for how the code is organized.

## Levels & Mechanics

50 levels split into 5 blocks of 10, each introducing a mechanic that recontextualizes everything learned before it:

| Block | Levels | Theme | Core mechanics |
|---|---|---|---|
| 1 | 1–10 | Basics | platforms, fake platforms, invisible walls, spikes, teleports, buttons, moving/disappearing platforms |
| 2 | 11–20 | Timing & Precision | retractable spikes, crumbling platforms, fake spikes |
| 3 | 21–30 | Gravity & Blades | gravity zones (low/high/reverse), pendulum blades |
| 4 | 31–40 | Darkness | vision that fades the more you move |
| 5 | 41–50 | Ice & Bounce | inertia-based ice platforms (no jumping), bounce pads |

17 distinct mechanics in total. Full breakdown in [MECHANICS.md](MECHANICS.md).

## Features

- Full 3D model pipeline (custom OBJ→X converter, see `tools/`) with block-specific themes per space-station biome
- Spherical skybox with procedural fallback, biome fog, twinkling starfield
- Player character with a real animation state machine (idle/run/jump/fall/death/celebration)
- Streamed background music per level block + SFX (jump, land, death, win, bounce, teleport), all volume-adjustable live in Settings
- 5-slot save/profile system: per-profile stars, best times, settings, unlockable cosmetic theme packs (alternate skyboxes + music, unlocked by star count)
- Full localization: English, Russian, Japanese (including dedicated font sets for Cyrillic/Kana)
- Star rating (1–3) per level based on completion time, F3 debug overlay (wireframes, block counts)

## Controls

| Key | Action |
|---|---|
| WASD | Move (camera-relative) |
| Mouse | Look / camera orbit |
| Space | Jump |
| E | Activate button/switch |
| R | Restart level |
| ESC | Pause / back |
| F3 | Debug overlay |

## Assets & Credits

- **3D models (all CC0):** [Quaternius](https://quaternius.com/) Ultimate Platformer Pack (player character, Block 1–2 terrain), [Kenney](https://kenney.nl/) Space Kit / Modular Space Kit / Space Station Kit and [KayKit](https://kaylousberg.com/) Space Base Bits (Block 3–5 terrain)
- **Music:** 5 original tracks generated with Suno AI, one per block
- **SFX:** [Freesound.org](https://freesound.org/), CC0
- **Font:** [Orbitron](https://fonts.google.com/specimen/Orbitron) (SIL OFL 1.1) for UI, plus Exo 2 / Noto Sans JP for Russian/Japanese localization

## Building

Open `MindTrapa.sln` in Visual Studio 2022, build `Debug|x86` or `Release|x86`. Requires the DxLib SDK (not included). No other external dependencies.

## Development History

See [DEVELOPMENT_HISTORY.md](DEVELOPMENT_HISTORY.md) for how the project actually got built — active development spanned ~13 months (June 2025 – July 2026) in 5 distinct work periods separated by breaks, not a single continuous push.
