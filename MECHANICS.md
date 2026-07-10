# Game Mechanics

All 17 mechanics are defined as `BlockType` values in `level.h` and driven by `Level::CheckCollision`/`Level::Update` in `level.cpp`. Levels are authored as plain C++ block lists in `level_block1.cpp`–`level_block5.cpp` (no data-driven level format — see [ARCHITECTURE.md](ARCHITECTURE.md)).

## Block 1 (Levels 1–10) — Basics

### PLATFORM
Standard solid block, full AABB collision. The baseline everything else subverts.

### FAKE_PLATFORM
Looks identical to `PLATFORM` but has no collision — the player falls straight through. The first "don't trust what you see" lesson.

### INVISIBLE_WALL
Solid collision, but not rendered (visible only via the F3 debug wireframe overlay). Blocks the obvious path and forces the player to find the real one.

### SPIKES
Instant death on contact (`Level::CheckDeadlyTrigger`). Rendered red.

### TELEPORT
Touching one teleport instantly moves the player to the linked one (matched by `linkId`), with a 1-second cooldown to prevent re-trigger loops (`Level::CheckTeleportTrigger`). Some levels use unpaired teleport IDs deliberately as traps.

### BUTTON
Press **E** within range to toggle `isActive`, which controls any block sharing its `linkId` (usually a `DISAPPEARING` door). Checked against both horizontal and vertical distance so it can't be triggered through a different floor (`Level::ActivateButton`).

### DISAPPEARING
Visible/solid when `isActive`, invisible/no-collision when not — either wired to a button, or auto-hides a fixed time after the player steps on it.

### MOVING
Platform that travels between two points; the player inherits its velocity while standing on it (including on fast vertical descent, handled via `savedPlatVel` in `Player.cpp` to avoid a one-frame desync).

### TRIGGER
The level exit. Reaching it completes the stage and advances to the next.

## Block 2 (Levels 11–20) — Timing & Precision

### RETRACTABLE_SPIKES
Extend/retract on a timer; only dangerous (and visible) while extended.

### CRUMBLING
Starts breaking the instant the player steps on it and disappears shortly after — must keep moving.

### FAKE_SPIKES
Visually identical to `SPIKES` but completely harmless. The central "obey the color, not the shape" lesson of Block 2 — used heavily in Level 14's fork/field/two-lane structure to force a real choice between a clean-looking dead end and a spike-covered safe path.

## Block 3 (Levels 21–30) — Gravity & Blades

### GRAVITY_ZONE
Multiplies gravity within its bounds: `<1.0` = floaty, `>1.0` = heavy, negative = reversed (walk on the "ceiling", handled by swapping floor/ceiling collision roles in `Level::CheckCollision`).

### PENDULUM_BLADE
A blade that swings on a sine-wave arc from a fixed pivot point; deadly on contact, telegraphed by a visible chain/pivot.

## Block 4 (Levels 31–40) — Darkness

### LIGHT_PULSE_ZONE
Periodically flashes light in an otherwise darkness-affected level. Darkness itself isn't a block type — it's a global `darknessLevel` that rises while the player moves and fades at rest (`main.cpp`), rendered as a screen-space overlay.

## Block 5 (Levels 41–50) — Ice & Bounce

### ICE_PLATFORM
Near-frictionless surface: input blends into persistent horizontal velocity instead of snapping directly (`Player::horizVel`, blend factor 0.04 with input, exponential decay without). Jumping is disabled while on ice — the only way off is a `BOUNCE_PAD`.

### BOUNCE_PAD
Launches the player upward at 1.5× normal jump velocity and explicitly clears `onGround`, regardless of which block the collision loop resolved first that frame.

## Controls

| Input | Action |
|---|---|
| WASD | Move, camera-relative |
| Mouse | Camera orbit (yaw/pitch, clamped) |
| Space | Jump (grounded only, disabled on ice) |
| E | Activate button (single-press, no repeat-spam) |
| R | Restart level |
| ESC | Pause / back |
| F3 | Debug overlay: colored wireframes per block type, block count |

## Physics

- Custom AABB collision, resolved on the smallest-overlap axis, contact side (floor vs. ceiling) decided from the player's *previous*-frame position rather than velocity sign (avoids a no-op case during fast falls/platform pushes)
- Gravity: `-0.015` units/frame², modified by `GRAVITY_ZONE` multiplier, symmetric fall-speed cap (`±0.6` units/frame) so reversed-gravity zones can't tunnel through thin colliders
- Jump power: `0.4` units/frame; full air control (no reduced air-strafe)
- Movement speed: `0.2` units/frame, deltaTime-normalized to 60 FPS baseline
