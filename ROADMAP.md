# Development Roadmap

## Current Version: Alpha 0.1
**Completed**: 6/100 levels, 9/50+ mechanics

---

## Phase 1: Core Gameplay (Levels 1-10)
**Goal**: Complete first block with all basic mechanics

### Sprint 1: Levels 7-10 ⏳ IN PROGRESS
- [ ] Level 7: Moving platforms introduction
- [ ] Level 8: Timed disappearing platforms
- [ ] Level 9: Combination - buttons + teleports
- [ ] Level 10: Mini-boss level (combines all block 1 mechanics)

**Estimated Time**: 2-3 sessions
**Priority**: HIGH

### Technical Tasks:
- [ ] Implement moving platform logic in `Level::Update()`
- [ ] Add timer system for disappearing platforms
- [ ] Create platform animation system
- [ ] Polish level designs based on playtesting

---

## Phase 2: Menu and Polish
**Goal**: Add basic UI and improve UX

### Sprint 2: Main Menu
- [ ] Create GameState enum (MENU, PLAYING, PAUSED)
- [ ] Main menu screen with options:
  - Start Game
  - Level Select
  - Exit
- [ ] Pause menu (ESC)
- [ ] Level selection screen (unlocks as you progress)

### Sprint 3: Visual Polish
- [ ] Add particle effects (death, teleport, button press)
- [ ] Improve camera smoothing
- [ ] Add screen shake on death
- [ ] Better lighting/shading

**Estimated Time**: 3-4 sessions
**Priority**: MEDIUM

---

## Phase 3: Content Expansion (Levels 11-30)
**Goal**: Add second and third blocks

### Block 2: Deadly Obstacles (Levels 11-20)
**Theme**: Timing and precision

- [ ] Spike traps with warning indicators
- [ ] Swinging axes
- [ ] Crushing walls
- [ ] Spike sequences (memorization)
- [ ] Fake spike sounds (trolling)

### Block 3: Puzzle Elements (Levels 21-30)
**Theme**: Logic and problem-solving

- [ ] Button sequence puzzles
- [ ] Multi-button gates
- [ ] Timed doors
- [ ] Weight-based pressure plates
- [ ] Mirror puzzles (buttons on both sides)

**Estimated Time**: 6-8 sessions
**Priority**: MEDIUM

---

## Phase 4: Advanced Mechanics (Levels 31-50)

### Block 4: Teleport Chaos (31-40)
- [ ] Random teleports
- [ ] Teleport chains (A→B→C→D)
- [ ] One-way teleports
- [ ] Fake teleports (look real but do nothing)
- [ ] Teleport mazes

### Block 5: Control Inversion (41-50)
- [ ] WASD inversion toggle
- [ ] Mouse inversion
- [ ] Gravity inversion (ceiling walking)
- [ ] Random inversion every N seconds
- [ ] Progressive inversion (gradually increases)

**Estimated Time**: 8-10 sessions
**Priority**: LOW

---

## Phase 5: Audio and Juice
**Goal**: Make game feel alive

### Sound Effects:
- [ ] Jump sound
- [ ] Landing sound
- [ ] Death sound (funny/trolling)
- [ ] Teleport whoosh
- [ ] Button click
- [ ] Spike trigger
- [ ] Level complete jingle

### Music:
- [ ] Menu music (calm, mysterious)
- [ ] Gameplay music (upbeat, slightly tense)
- [ ] Boss level music (intense)
- [ ] Victory fanfare

### Implementation:
- [ ] Add audio manager class
- [ ] Use DxLib audio functions
- [ ] Volume controls in settings

**Estimated Time**: 2-3 sessions
**Priority**: MEDIUM

---

## Phase 6: Content Completion (Levels 51-100)

### Block 6: Visual Deception (51-60)
- [ ] Extensive use of fake platforms
- [ ] Fake walls
- [ ] Optical illusions
- [ ] Flickering visibility

### Block 7: Time Mechanics (61-70)
- [ ] Slow-motion zones
- [ ] Fast-forward zones
- [ ] Time-limited platforms
- [ ] Rewind mechanic (advanced)

### Block 8: Physics Chaos (71-80)
- [ ] Variable gravity zones
- [ ] Wind/conveyor belts
- [ ] Bouncy platforms
- [ ] Ice physics

### Block 9: Psychological Trolling (81-90)
- [ ] Fake win conditions
- [ ] Maze levels
- [ ] Multiple fake exits
- [ ] Backwards progression

### Block 10: Final Challenge (91-100)
- [ ] Combines all mechanics
- [ ] Speed challenges
- [ ] Memory challenges
- [ ] Hidden true ending
- [ ] Secret levels

**Estimated Time**: 15-20 sessions
**Priority**: LOW

---

## Phase 7: Models and Visuals
**Goal**: Replace cubes with proper 3D models

### Tasks:
- [ ] Create/acquire 3D models (or use simple shapes)
- [ ] Model for player character
- [ ] Platform models (stone, metal, wood)
- [ ] Spike models
- [ ] Teleport portal effect
- [ ] Button/switch models
- [ ] Particle effects (dust, sparks, etc.)

### Considerations:
- Keep it simple - stylized low-poly is fine
- Performance: ensure 60fps with models
- May need to learn 3D modeling or find free assets

**Estimated Time**: 5-7 sessions
**Priority**: LOW (cosmetic)

---

## Phase 8: Level Editor (Advanced)
**Goal**: Allow easy level creation and testing

### Features:
- [ ] Visual level editor
- [ ] Place/delete blocks with mouse
- [ ] Set block properties (type, linkId, etc.)
- [ ] Save/load levels to files
- [ ] Playtest mode (instant test)
- [ ] Level validation (reachability check)

### Format:
```json
{
  "levelId": 1,
  "playerSpawn": [0, 1, 0],
  "blocks": [
    {"type": "PLATFORM", "pos": [0, 0, 0], "size": [10, 1, 10]},
    ...
  ]
}
```

**Estimated Time**: 10+ sessions
**Priority**: LOW (nice-to-have)

---

## Phase 9: Release Preparation

### Polish:
- [ ] Credits screen
- [ ] Settings menu (volume, controls)
- [ ] Tutorial tooltips
- [ ] Death quotes/messages (trolling flavor)
- [ ] Statistics (total deaths, time played)
- [ ] Achievements (optional)

### Testing:
- [ ] Full playthrough testing
- [ ] Difficulty balancing
- [ ] Bug fixes
- [ ] Performance optimization

### Distribution:
- [ ] Create installer
- [ ] Write game description
- [ ] Prepare marketing materials (screenshots, gif)
- [ ] Publish on itch.io or similar

**Estimated Time**: 3-5 sessions
**Priority**: LOW

---

## Future Ideas (Post-1.0)

### Post-Launch Content:
- [ ] Daily challenge levels
- [ ] Level sharing/community levels
- [ ] Speedrun mode with timer
- [ ] Hard mode (one life for entire game)
- [ ] Secret alternate endings

### Technical Improvements:
- [ ] Port to other platforms (Linux, Mac)
- [ ] Add gamepad support
- [ ] Implement replay system
- [ ] Add mod support

---

## Development Principles

1. **Gameplay First**: Mechanics and level design before visuals
2. **Iterative Testing**: Playtest frequently, adjust difficulty
3. **Keep it Fun**: If something isn't fun, cut it or rework it
4. **Document Everything**: Make it easy to return after breaks
5. **One Feature at a Time**: Finish what you start
6. **Trolling with Love**: Challenge players, don't frustrate them

---

## Time Estimates

**Conservative Estimate**: 50-60 development sessions (2-3 hours each)
**Optimistic Estimate**: 30-40 sessions with focused work
**Total Development Time**: 6-12 months part-time

**Current Progress**: ~10 sessions completed (~15-20% of MVP)

---

## Success Metrics

**MVP (Minimum Viable Product)**:
- [ ] 30 levels (blocks 1-3)
- [ ] All core mechanics working
- [ ] Basic menu
- [ ] Sound effects

**Full Release v1.0**:
- [ ] 100 levels (all 10 blocks)
- [ ] All mechanics implemented
- [ ] Complete audio
- [ ] 3D models
- [ ] Level editor

**Dream Version v2.0**:
- [ ] Community level sharing
- [ ] Speedrun leaderboards
- [ ] Mod support
- [ ] Additional DLC blocks