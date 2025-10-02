# Game Mechanics

## ✅ Implemented Mechanics

### **1. PLATFORM (Normal)**
- **Type**: `BlockType::PLATFORM`
- **Behavior**: Solid block, standard collision
- **Visual**: Gray cube
- **Levels Used**: All levels

### **2. TRIGGER (Level Completion)**
- **Type**: `BlockType::TRIGGER`  
- **Behavior**: Touching triggers level completion → next level
- **Visual**: Cyan/blue cube
- **Levels Used**: All levels

### **3. SPIKES (Deadly)**
- **Type**: `BlockType::SPIKES`
- **Behavior**: Instant death on contact → restart level
- **Visual**: Red cube
- **Levels Used**: Level 2, 5
- **Code**: `CheckDeadlyTrigger()`

### **4. INVISIBLE_WALL**
- **Type**: `BlockType::INVISIBLE_WALL`
- **Behavior**: Solid collision, invisible (or debug outline)
- **Visual**: Red wireframe (debug mode)
- **Levels Used**: Level 3
- **Trolling**: Blocks obvious path, forces player to find alternate route

### **5. FAKE_PLATFORM**
- **Type**: `BlockType::FAKE_PLATFORM`
- **Behavior**: Looks solid but no collision - player falls through
- **Visual**: Gray cube (identical to normal platform)
- **Levels Used**: Planned for future levels
- **Trolling**: Visual deception

### **6. TELEPORT (Portal)**
- **Type**: `BlockType::TELEPORT`
- **Behavior**: 
  - Touch teleport → instantly move to linked teleport
  - Linked by `linkId` parameter
  - 1-second cooldown to prevent spam
- **Visual**: Purple/magenta cube
- **Levels Used**: Level 4, 5
- **Code**: `CheckTeleportTrigger()`
- **Variations**:
  - **Simple A→B**: Two teleports with same linkId
  - **Trap teleport**: Teleports to spikes or void
  - **Multi-choice**: Multiple teleports, only one correct

### **7. BUTTON (Switch)**
- **Type**: `BlockType::BUTTON`
- **Behavior**: 
  - Press E when close (3 unit radius) → toggle active state
  - Activates linked blocks with same `linkId`
  - Can be toggled on/off repeatedly
- **Visual**: 
  - Yellow = inactive
  - Green = active
- **Levels Used**: Level 6
- **Code**: `ActivateButton()`
- **Linked Objects**: Usually opens doors (DISAPPEARING blocks)

### **8. DISAPPEARING (Door/Timed Platform)**
- **Type**: `BlockType::DISAPPEARING`
- **Behavior**:
  - If `isActive = true` → visible and solid
  - If `isActive = false` → invisible and no collision
  - Can be controlled by buttons (linkId)
  - Future: Timer-based disappearing
- **Visual**: Yellow cube (when active)
- **Levels Used**: Level 6 (as door)
- **Future Use**: Timed platforms that disappear after X seconds

### **9. MOVING (Future)**
- **Type**: `BlockType::MOVING`
- **Behavior**: Platform moves between two points
- **Status**: ⏳ Not yet implemented
- **Planned**: Use `moveTarget` vector for destination

## 📋 Planned Mechanics

### **Block 1-10: Basic Trolling**
- [ ] Double jump mechanic
- [ ] Moving platforms
- [ ] Crumbling platforms (disappear after stepping)

### **Block 11-20: Advanced Traps**
- [ ] Spike traps (pop up with warning)
- [ ] Timed spike sequences
- [ ] Fake spike sounds

### **Block 21-30: Puzzle Elements**
- [ ] Multiple buttons → sequence puzzles
- [ ] Timed doors (close after X seconds)
- [ ] Pressure plates (must stay on)

### **Block 31-40: Teleport Chaos**
- [ ] Random teleports (unpredictable destination)
- [ ] Teleport chains (A→B→C)
- [ ] One-way teleports

### **Block 41-50: Control Inversion**
- [ ] Inverted WASD (W=back, S=forward)
- [ ] Inverted mouse (up=down)
- [ ] Inverted gravity (walk on ceiling)
- [ ] Random inversion every 5 seconds

### **Block 51-60: Visual Deception**
- [ ] More fake platforms
- [ ] Fake walls (can walk through)
- [ ] Optical illusions (far looks near)
- [ ] Flickering platforms (visible/invisible)

### **Block 61-70: Time Mechanics**
- [ ] Slow motion zones
- [ ] Fast forward zones
- [ ] Rewind mechanic
- [ ] Time-frozen platforms

### **Block 71-80: Physics Chaos**
- [ ] Low gravity zones
- [ ] High gravity zones
- [ ] Wind/conveyor platforms
- [ ] Bouncy platforms

### **Block 81-90: Psychological**
- [ ] Fake win triggers
- [ ] Maze with identical rooms
- [ ] Multiple fake exits
- [ ] Backwards level progression

### **Block 91-100: Final Boss**
- [ ] Combination of all mechanics
- [ ] Speed run challenges
- [ ] Memory challenges
- [ ] Hidden true ending

## 🎮 Control Mechanics

### **Movement**
- WASD: Directional movement (relative to camera)
- Mouse: Camera rotation (yaw and pitch)
- Movement speed: 0.2 units/frame
- Camera distance: 10 units from player

### **Jump**
- Space: Jump (only when onGround)
- Jump power: 0.4 units/frame
- Gravity: -0.015 units/frame²
- Future: Double jump, wall jump

### **Interaction**
- E: Activate button/switch
- Activation radius: 3 units
- One press = one toggle (no spam)

### **System**
- R: Restart current level
- ESC: Quit game
- Future: Pause menu

## 🔧 Technical Implementation Notes

### **Collision Detection**
- **Type**: AABB (Axis-Aligned Bounding Box)
- **Axes**: Full 3D (X, Y, Z)
- **Resolution**: Push player out on smallest overlap axis
- **Edge Cases**: Velocity direction check prevents falling through

### **Teleportation**
- **Cooldown**: 1.0 second to prevent infinite loops
- **Velocity Reset**: Zero velocity after teleport
- **Position**: Center of destination teleport + offset

### **Button-Door Linking**
- **System**: `linkId` integer links objects
- **Example**: Button with linkId=10 controls all DISAPPEARING blocks with linkId=10
- **Scalability**: Can link multiple buttons to one door, or one button to multiple doors

### **Active State**
- **Boolean**: `isActive` controls visibility and collision
- **BUTTON**: false=yellow, true=green
- **DISAPPEARING**: false=invisible/no collision, true=visible/solid

## 📊 Difficulty Progression

**Level 1-3**: Tutorial (basic movement, simple jumps)
**Level 4-6**: Introduction (teleports, buttons, basic trolling)
**Level 7-10**: Easy trolling (combine mechanics)
**Level 11-20**: Medium difficulty (timing, precision)
**Level 21-50**: Hard (complex puzzles, sequence challenges)
**Level 51-80**: Very Hard (muscle memory, reflexes)
**Level 81-100**: Expert (everything combined, minimal hints)