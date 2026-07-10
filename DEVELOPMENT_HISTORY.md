# Development History

MindTrapa was developed solo, part-time, alongside full-time study — active work happened in five distinct periods over about 13 months, separated by multi-week/month breaks (academic terms). Reconstructed from git commit history (33 commits, first 2025-06-17, last 2026-07-10).

| Period | Dates | What shipped |
|---|---|---|
| 1 | Jun 2025 | Camera and player-control prototype |
| 2 | Oct–Nov 2025 | Block 1 + Block 2 complete (levels 1–20), core mechanics, menu navigation |
| 3 | Jan 2026 | Block 3 complete (levels 21–30): gravity zones, pendulum blades |
| 4 | Apr–May 2026 | Block 4 (levels 31–40), full 3D model pipeline, player character + animations, profile/save system, BGM/SFX |
| 5 | Jun–Jul 2026 | Block 5 complete (levels 41–50), refactor, localization (EN/RU/JA), theme packs, full audit pass, final polish |

Roughly 4 months of active, concentrated work spread across a 13-month calendar span.

## How the scope actually changed

Early planning (visible in the original design notes) sketched a much larger, genre-agnostic "trolling platformer" — 100 levels across 10 blocks, with mechanics like control inversion, time manipulation, and a level editor. What actually shipped is smaller and more coherent:

- **100 → 50 levels, 10 → 5 blocks.** Cutting scope in half in favor of finishing and polishing rather than leaving later blocks half-built.
- **Generic "trolling" framing → a consistent space theme.** Every block is a distinct space biome (asteroid/station/ice) with matching skybox, fog, and music, rather than an arbitrary mechanic grab-bag.
- **Placeholder cubes → a real 3D pipeline.** The original plan treated models as optional polish; the shipped game has a full asset pipeline (custom OBJ→X converter, per-block model theming, a player character with a full animation state machine).
- **No localization in the original plan → EN/RU/JA support**, including separate font sets for Cyrillic and Kana.
- **No save system planned → 5-slot profile system** with stars, best times, and unlockable cosmetic theme packs.

The net effect: less breadth than originally sketched, but everything that shipped is finished, tested, and polished — not a pile of half-implemented "planned" mechanics.

## Final pass

The last active period (Jun–Jul 2026) was a dedicated audit-and-polish sprint: three independent code reviews surfaced 36 findings (2 critical, 7 high, 14 medium, 13 low), all fixed and verified before calling the project 1.0. Notable fixes from that pass: a collision edge case that could leave the player embedded in geometry for a frame during fast falls, an uncapped fall speed that could tunnel through thin colliders under reversed gravity, and a one-frame camera/player desync most visible on long falls.
