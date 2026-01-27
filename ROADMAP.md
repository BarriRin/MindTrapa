# Development Roadmap

## Current Version: Alpha 0.4
**Completed**: 30/50 levels (60%), 14 mechanics
**Current Focus**: 3D Models & Block 4 Design

---

## ✅ Phase 1: Block 1 - Basics (Levels 1-10) - COMPLETE
**Status**: ✅ 100% Complete

### Implemented Mechanics:
- ✅ Basic movement and jumps
- ✅ Teleports
- ✅ Buttons and doors
- ✅ Moving platforms
- ✅ Invisible walls
- ✅ Static spikes

**Result**: 10 levels introducing core platforming concepts

---

## ✅ Phase 2: Block 2 - Timing & Precision (Levels 11-20) - COMPLETE
**Status**: ✅ 100% Complete

### Implemented Mechanics:
- ✅ Retractable spikes (timed danger)
- ✅ Crumbling platforms (fall after touch)
- ✅ Fake spikes (pink, harmless trolling)
- ✅ Combination challenges

**Result**: 10 levels focused on timing and precision gameplay

---

## ✅ Phase 3: Block 3 - Gravity & Blades (Levels 21-30) - COMPLETE
**Theme**: Jupiter (high gravity, turbulent atmosphere)
**Status**: ✅ 100% Complete

### Implemented Mechanics:
- ✅ **Gravity Zones**:
  - Low gravity (floaty jumps, slow fall)
  - High gravity (heavy jumps, fast fall)
  - Reverse gravity (walk on ceiling)
  - Transition zones (gradual gravity change)

- ✅ **Pendulum Blades**:
  - Swinging axe/blade obstacles
  - Predictable timing (sine wave motion)
  - Various speeds and ranges
  - Deadly on contact

### Completed Levels (21-30):
- ✅ Level 21: Introduce low gravity (tutorial)
- ✅ Level 22: Introduce high gravity
- ✅ Level 23: Introduce pendulum blades (simple)
- ✅ Level 24: Combine gravity + blades
- ✅ Level 25: Reverse gravity introduction
- ✅ Level 26: Complex pendulum patterns
- ✅ Level 27: Multi-gravity zones + blades
- ✅ Level 28: Trolling with gravity (unexpected switches)
- ✅ Level 29: Blade gauntlet + gravity puzzle
- ✅ Level 30: Block 3 finale (all mechanics combined)

**Result**: 10 levels with gravity manipulation and pendulum blade mechanics

---

## Phase 4: Block 4 (Levels 31-40) - TO BE DESIGNED
**Theme**: TBD
**Goal**: New unique mechanic set

### Ideas Under Consideration:
- Portal mechanics 2.0 (momentum-based)
- Phase shifting (two dimensions)
- Laser/beam puzzles
- Conveyor belts and wind
- Time manipulation

**Status**: Design phase
**Priority**: MEDIUM

---

## Phase 5: Block 5 - Final Challenge (Levels 41-50)
**Theme**: The Gauntlet - Ultimate Test
**Goal**: Combine ALL mechanics from blocks 1-4

### Concept:
- Progressive difficulty curve
- Each level uses mechanics from multiple blocks
- Level 50: Epic finale combining everything
- Secret/bonus challenges

**Status**: Future planning
**Priority**: LOW

---

## Phase 6: Polish and Audio
**Goal**: Make game feel professional

### UI/UX Improvements:
- [x] Main menu system
- [x] Pause menu
- [x] Level select
- [ ] Settings menu (volume, controls)
- [ ] Tutorial tooltips
- [ ] Death counter display

### Audio:
- [ ] Sound effects (jump, death, teleport, etc.)
- [ ] Background music per block
- [ ] Volume controls
- [ ] Audio manager class

### Visual Polish:
- [ ] Particle effects (death, teleport)
- [ ] Screen shake
- [ ] Better camera smoothing
- [ ] Lighting improvements

**Priority**: MEDIUM
**Estimated Time**: 3-4 sessions

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

**Current Progress**: MVP core complete (30 levels), entering polish phase

---

## Success Metrics

**MVP (Minimum Viable Product)**:
- [x] 30 levels (blocks 1-3)
- [x] All core mechanics working
- [x] Basic menu
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