#include "Level.h"

Level::Level(int id) : levelId(id) {
    // Derive the level-block ID from the level number (10 levels per block)
    blockId = (id - 1) / 10;

    LoadLevelData(id);
    InitializeModels();
}

Level::~Level() {
    // Release all duplicated block models
    for (auto& block : blocks) {
        if (block.modelHandle != -1) {
            MV1DeleteModel(block.modelHandle);
        }
    }
    blocks.clear();
}

void Level::LoadLevelData(int id) {
    blocks.clear();

    if (id >= 1 && id <= 10) { LoadBlock1(id); return; }
    if (id >= 11 && id <= 20) { LoadBlock2(id); return; }
    if (id >= 21 && id <= 30) { LoadBlock3(id); return; }
    if (id >= 31 && id <= 40) { LoadBlock4(id); return; }
    if (id >= 41 && id <= 50) { LoadBlock5(id); return; }

    // Fallback for unknown levels
    playerSpawn = VGet(0, 1, 0);
    blocks.push_back(Block(VGet(-3, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));
    blocks.push_back(Block(VGet(5, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
}

static unsigned int GetDebugColor(BlockType type) {
    switch (type) {
    case BlockType::PLATFORM:           return GetColor(255, 255, 255);
    case BlockType::FAKE_PLATFORM:      return GetColor(170, 170, 170);
    case BlockType::INVISIBLE_WALL:     return GetColor(255, 136,   0);
    case BlockType::SPIKES:             return GetColor(255,  51,  51);
    case BlockType::FAKE_SPIKES:        return GetColor(255, 136, 170);
    case BlockType::RETRACTABLE_SPIKES: return GetColor(204,  34,   0);
    case BlockType::DISAPPEARING:       return GetColor(255, 255,   0);
    case BlockType::CRUMBLING:          return GetColor(255, 136,  51);
    case BlockType::MOVING:             return GetColor(  0, 255,  68);
    case BlockType::BUTTON:             return GetColor(255,   0, 255);
    case BlockType::TELEPORT:           return GetColor(204,   0, 255);
    case BlockType::TRIGGER:            return GetColor(  0, 255, 255);
    case BlockType::GRAVITY_ZONE:       return GetColor(136,  68, 255);
    case BlockType::PENDULUM_BLADE:     return GetColor(255,  34,  34);
    case BlockType::LIGHT_PULSE_ZONE:   return GetColor(204, 102, 255);
    case BlockType::ICE_PLATFORM:       return GetColor(136, 204, 255);
    case BlockType::BOUNCE_PAD:         return GetColor(255, 170,   0);
    default:                            return GetColor(255, 255, 255);
    }
}

void Level::Draw(bool debugMode) const {
    for (const auto& block : blocks) {
        // If the block has a model, draw the model (respecting visibility rules).
        // Some types always use primitives instead (INVISIBLE_WALL, zones).
        if (block.useModel && block.modelHandle != -1 &&
            block.type != BlockType::INVISIBLE_WALL &&
            block.type != BlockType::GRAVITY_ZONE &&
            block.type != BlockType::LIGHT_PULSE_ZONE) {

            // Visibility check for stateful blocks
            bool visible = true;
            if ((block.type == BlockType::DISAPPEARING || block.type == BlockType::CRUMBLING) && !block.isActive)
                visible = false;  // gone/fallen — don't draw
            if (block.type == BlockType::RETRACTABLE_SPIKES && !block.isActive)
                visible = false;  // spikes retracted — don't draw
            if (block.type == BlockType::SPIKES && block.linkId == 1 && !block.isActive)
                visible = false;  // SPIKES(linkId=1) — retractable variant, hidden while inactive

            if (visible) {
                // Models whose origin sits at their base get placed at the block's
                // bottom face (X/Z centered, Y at the bottom)
                VECTOR modelPos;
                if (block.type == BlockType::TRIGGER)
                    modelPos = VGet(block.pos.x + block.size.x * 0.5f, block.pos.y, block.pos.z + block.size.z * 0.5f);
                else
                    modelPos = VAdd(block.pos, VScale(block.size, 0.5f));

                MV1SetPosition(block.modelHandle, modelPos);
                MV1SetScale(block.modelHandle, block.modelScale);
                MV1SetRotationXYZ(block.modelHandle, block.rotation);
                MV1DrawModel(block.modelHandle);

                // Hazard_Cylinder is a half-cylinder — draw the mirrored half too (180° around Y)
                if (block.type == BlockType::PENDULUM_BLADE) {
                    MV1SetRotationXYZ(block.modelHandle,
                        VGet(block.rotation.x, block.rotation.y + DX_PI_F, block.rotation.z));
                    MV1DrawModel(block.modelHandle);
                }
            }
            continue;
        }

        // Otherwise fall back to primitives (current implementation)
        switch (block.type) {
        case BlockType::PLATFORM:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(200, 200, 200), GetColor(150, 150, 150), TRUE);
            break;

        case BlockType::TRIGGER:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(100, 200, 255), GetColor(50, 150, 200), TRUE);
            break;

        case BlockType::INVISIBLE_WALL:
            break;

        case BlockType::FAKE_PLATFORM:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(180, 180, 180), GetColor(130, 130, 130), TRUE);
            break;

        case BlockType::SPIKES:
            if (block.linkId == 1) {
                if (block.isActive) {
                    DrawCube3D(block.pos, VAdd(block.pos, block.size),
                        GetColor(255, 50, 50), GetColor(200, 0, 0), TRUE);
                }
                else {
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
                    DrawCube3D(block.pos, VAdd(block.pos, block.size),
                        GetColor(150, 150, 150), GetColor(100, 100, 100), TRUE);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                }
            }
            else {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 50, 50), GetColor(200, 0, 0), TRUE);
            }
            break;

        case BlockType::DISAPPEARING:
            if (block.isActive) {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 255, 100), GetColor(200, 200, 50), TRUE);
            }
            break;

        case BlockType::MOVING:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
            break;

        case BlockType::BUTTON:
            if (block.isActive) {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
            }
            else {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 255, 100), GetColor(200, 200, 50), TRUE);
            }
            break;

        case BlockType::TELEPORT:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 100, 255), GetColor(200, 50, 200), TRUE);
            break;

        case BlockType::RETRACTABLE_SPIKES:
            // Retractable spikes - only visible while active
            if (block.isActive) {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 50, 50), GetColor(200, 0, 0), TRUE);
            }
            // else: fully hidden while retracted
            break;

        case BlockType::CRUMBLING:
            // Crumbling platforms - yellow-brown
            if (block.isActive) {
                // Normal state
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(200, 150, 50), GetColor(150, 100, 30), TRUE);
            }
            else {
                // Falling/gone (semi-transparent)
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(200, 150, 50), GetColor(150, 100, 30), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }
            break;

        case BlockType::FAKE_SPIKES:
            // Fake spikes - PINK (look dangerous, but are harmless)
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 100, 150), GetColor(200, 50, 100), TRUE);
            break;

        case BlockType::GRAVITY_ZONE:
            // Gravity zones - very transparent, color depends on the gravity type
            unsigned int gravityColor;
            if (block.gravityMultiplier < 0.0f) {
                // Reverse gravity - purple
                gravityColor = GetColor(200, 100, 255);
            } else if (block.gravityMultiplier < 1.0f) {
                // Low gravity - light blue
                gravityColor = GetColor(100, 200, 255);
            } else if (block.gravityMultiplier > 1.0f) {
                // High gravity - orange
                gravityColor = GetColor(255, 150, 50);
            } else {
                // Normal (shouldn't happen, but just in case)
                gravityColor = GetColor(150, 150, 150);
            }
            // Disable Z-buffer writes so the zone doesn't occlude objects inside it
            SetWriteZBuffer3D(FALSE);
            // Very transparent - alpha 50 out of 255 (~20% opacity)
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                gravityColor, gravityColor, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            // Restore Z-buffer writes
            SetWriteZBuffer3D(TRUE);
            break;

        case BlockType::PENDULUM_BLADE:
            // Pendulum blade - draw the chain (or rope) and the blade
            // Chain from the pivot point to the blade
            DrawLine3D(block.pivotPoint, block.pos, GetColor(100, 100, 100));

            // Blade - red cube (placeholder until there's a model)
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(200, 50, 50), GetColor(150, 30, 30), TRUE);

            // Pivot point - small sphere (simplified as a cube)
            VECTOR pivotSize = VGet(0.3f, 0.3f, 0.3f);
            DrawCube3D(VGet(block.pivotPoint.x - 0.15f, block.pivotPoint.y - 0.15f, block.pivotPoint.z - 0.15f),
                VGet(block.pivotPoint.x + 0.15f, block.pivotPoint.y + 0.15f, block.pivotPoint.z + 0.15f),
                GetColor(80, 80, 80), GetColor(60, 60, 60), TRUE);
            break;

        case BlockType::LIGHT_PULSE_ZONE:
            // Flickering light zone - very transparent so the player can see its bounds
            SetWriteZBuffer3D(FALSE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 25);
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(80, 60, 120), GetColor(80, 60, 120), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            SetWriteZBuffer3D(TRUE);
            break;

        case BlockType::ICE_PLATFORM:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(160, 210, 250), GetColor(110, 165, 225), TRUE);
            break;

        case BlockType::BOUNCE_PAD:
            // Bounce pad - yellow-orange
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 200, 50), GetColor(220, 150, 20), TRUE);
            break;
        }
    }

    // Persistent wireframe on Block 3+ platforms (space aesthetic)
    if (blockId >= 2) {
        for (const auto& block : blocks) {
            if (block.type == BlockType::PLATFORM || block.type == BlockType::FAKE_PLATFORM ||
                block.type == BlockType::MOVING ||
                (block.type == BlockType::CRUMBLING && block.isActive)) {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
            }
        }
    }

    if (debugMode) {
        SetUseZBuffer3D(FALSE);  // disable Z-test so the outlines show through the geometry
        for (const auto& block : blocks) {
            if (!block.isActive && (block.type == BlockType::DISAPPEARING ||
                block.type == BlockType::CRUMBLING ||
                block.type == BlockType::RETRACTABLE_SPIKES ||
                (block.type == BlockType::SPIKES && block.linkId == 1))) continue;
            unsigned int col = GetDebugColor(block.type);
            DrawCube3D(block.pos, VAdd(block.pos, block.size), col, col, FALSE);
        }
        SetUseZBuffer3D(TRUE);
    }
}

bool Level::CheckCollision(VECTOR playerPos, VECTOR playerSize, VECTOR& newPos, VECTOR& velocity, bool& onGround, VECTOR& platformVelocity, bool gravityReversed, bool* justBounced) {
    onGround = false;
    platformVelocity = VGet(0, 0, 0);
    bool collisionOccurred = false;
    bool bounced = false; // flag: the bounce pad already fired this frame — the floor branch must not zero velocity.y
    if (justBounced) *justBounced = false;

    for (auto& block : blocks) {
        if (block.type == BlockType::PLATFORM ||
            (block.type == BlockType::INVISIBLE_WALL && block.isActive) ||
            block.type == BlockType::MOVING ||
            block.type == BlockType::BUTTON ||
            (block.type == BlockType::DISAPPEARING && block.isActive) ||
            (block.type == BlockType::CRUMBLING && block.isActive) ||
            block.type == BlockType::ICE_PLATFORM ||
            block.type == BlockType::BOUNCE_PAD) {

            if (newPos.x < block.pos.x + block.size.x && newPos.x + playerSize.x > block.pos.x &&
                newPos.y < block.pos.y + block.size.y && newPos.y + playerSize.y > block.pos.y &&
                newPos.z < block.pos.z + block.size.z && newPos.z + playerSize.z > block.pos.z) {

                float overlapX = 0, overlapY = 0, overlapZ = 0;

                if (newPos.x < block.pos.x) {
                    overlapX = (newPos.x + playerSize.x) - block.pos.x;
                }
                else {
                    overlapX = (block.pos.x + block.size.x) - newPos.x;
                }

                if (newPos.y < block.pos.y) {
                    overlapY = (newPos.y + playerSize.y) - block.pos.y;
                }
                else {
                    overlapY = (block.pos.y + block.size.y) - newPos.y;
                }

                if (newPos.z < block.pos.z) {
                    overlapZ = (newPos.z + playerSize.z) - block.pos.z;
                }
                else {
                    overlapZ = (block.pos.z + block.size.z) - newPos.z;
                }

                if (overlapY <= overlapX && overlapY <= overlapZ) {
                    // Which side we hit is decided from the player's OLD position, not
                    // the sign of velocity.y — otherwise a fast fall or a platform push
                    // could leave the old position matching neither "landed" nor
                    // "hit ceiling": collisionOccurred would still be set, but newPos.y
                    // never got corrected, and the player sank into the geometry for a frame.
                    bool cameFromAbove = playerPos.y > block.pos.y + block.size.y - 1.5f;
                    // In reverse-gravity zones the "floor" is physically overhead — the
                    // player approaches it from below, so the two sides swap roles.
                    bool isFloorContact = gravityReversed ? !cameFromAbove : cameFromAbove;

                    if (cameFromAbove) newPos.y = block.pos.y + block.size.y;
                    else                newPos.y = block.pos.y - playerSize.y;

                    if (isFloorContact) {
                        if (block.type == BlockType::BOUNCE_PAD) {
                            // The bounce pad overrides the outcome regardless of block
                            // order in the list: if a regular platform already set
                            // onGround this same frame, the bounce pad clears it —
                            // otherwise we'd end up with onGround=true and a nonzero
                            // velocity.y, which is a contradictory state.
                            if (!bounced) {
                                velocity.y = gravityReversed ? -0.6f : 0.6f;
                                bounced = true;
                                onGround = false;
                                if (justBounced) *justBounced = true;
                            }
                        } else if (gravityReversed ? (velocity.y >= 0) : (velocity.y <= 0)) {
                            if (!bounced) { velocity.y = 0; onGround = true; }
                        }

                        if (block.type == BlockType::MOVING) {
                            platformVelocity = VGet(
                                block.pos.x - block.prevPos.x,
                                block.pos.y - block.prevPos.y,
                                block.pos.z - block.prevPos.z
                            );
                        }

                        if (block.type == BlockType::DISAPPEARING && block.linkId == 0) {
                            if (block.timer == 0.0f) {
                                block.timer = 1.5f;
                            }
                        }

                        if (block.type == BlockType::CRUMBLING) {
                            if (block.timer == 0.0f) {
                                block.timer = 0.5f * block.moveSpeed;
                            }
                        }
                    }
                    else {
                        if (!bounced) velocity.y = 0;
                    }
                }
                else if (overlapX <= overlapZ) {
                    if (newPos.x < block.pos.x) {
                        newPos.x = block.pos.x - playerSize.x;
                    }
                    else {
                        newPos.x = block.pos.x + block.size.x;
                    }
                }
                else {
                    if (newPos.z < block.pos.z) {
                        newPos.z = block.pos.z - playerSize.z;
                    }
                    else {
                        newPos.z = block.pos.z + block.size.z;
                    }
                }

                collisionOccurred = true;
                // Keep iterating — bounce pads on other platforms still need checking
            }
        }
    }

    return collisionOccurred;
}

bool Level::CheckWinTrigger(VECTOR playerPos, VECTOR playerSize) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::TRIGGER) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return true;
            }
        }
    }
    return false;
}

bool Level::CheckDeadlyTrigger(VECTOR playerPos, VECTOR playerSize) const {
    for (const auto& block : blocks) {
        // Regular spikes
        if (block.type == BlockType::SPIKES) {
            if (block.linkId == 1 && !block.isActive) {
                continue;
            }

            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return true;
            }
        }

        // Retractable spikes - only deadly while extended
        if (block.type == BlockType::RETRACTABLE_SPIKES && block.isActive) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return true;
            }
        }

        // Pendulum blades (Block 3) - always deadly
        if (block.type == BlockType::PENDULUM_BLADE) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return true;
            }
        }

        // FAKE_SPIKES don't kill - that's the trick!
    }
    return false;
}

void Level::Update(float deltaTime) {
    for (auto& block : blocks) {
        if (block.type == BlockType::MOVING) {
            block.prevPos = block.pos;

            block.timer += deltaTime * 0.3f;

            if (block.timer > 1.0f) {
                block.timer = 1.0f;
                block.isActive = false;
            }
            if (block.timer < 0.0f) {
                block.timer = 0.0f;
                block.isActive = true;
            }

            if (!block.isActive) {
                block.timer -= deltaTime * 0.6f;
            }

            float t = block.timer;
            block.pos.x = block.originalPos.x + (block.moveTarget.x - block.originalPos.x) * t;
            block.pos.y = block.originalPos.y + (block.moveTarget.y - block.originalPos.y) * t;
            block.pos.z = block.originalPos.z + (block.moveTarget.z - block.originalPos.z) * t;
        }

        if (block.type == BlockType::DISAPPEARING) {
            if (block.linkId == 0 && block.timer > 0.0f) {
                block.timer -= deltaTime;

                if (block.timer <= 0.0f) {
                    block.isActive = false;
                    block.timer = -3.0f;
                }
            }

            if (block.linkId == 0 && block.timer < 0.0f) {
                block.timer += deltaTime;

                if (block.timer >= 0.0f) {
                    block.isActive = true;
                    block.timer = 0.0f;
                }
            }
        }

        if (block.type == BlockType::SPIKES && block.linkId == 1) {
            block.timer += deltaTime;

            float cycleTime = 4.0f;
            float phase = fmod(block.timer, cycleTime);

            if (phase < 2.0f) {
                block.isActive = true;
            }
            else {
                block.isActive = false;
            }
        }

        // RETRACTABLE_SPIKES - extend/retract on a timer
        if (block.type == BlockType::RETRACTABLE_SPIKES) {
            block.timer += deltaTime;

            // speed=1.0 -> 4s cycle (2s up + 2s down), speed=2.0 -> 2s cycle
            float cycleTime = 4.0f / block.moveSpeed;
            float halfCycle = cycleTime / 2.0f;
            float phase = fmod(block.timer, cycleTime);

            block.isActive = (phase < halfCycle); // true = spikes extended (dangerous)
        }

        // CRUMBLING - platform breaks apart after the player touches it
        // (the touch detection itself lives in CheckCollision)
        if (block.type == BlockType::CRUMBLING) {
            // If the platform has started crumbling (timer > 0)
            if (block.timer > 0.0f) {
                block.timer -= deltaTime;

                // Disappears after 0.5s
                if (block.timer <= 0.0f) {
                    block.isActive = false; // Platform has fallen
                    block.timer = -2.0f;    // Time until respawn
                }
            }

            // Platform is gone, waiting to respawn
            if (block.timer < 0.0f) {
                block.timer += deltaTime;

                // Comes back after 2s
                if (block.timer >= 0.0f) {
                    block.isActive = true;
                    block.timer = 0.0f;
                }
            }
        }

        // LIGHT_PULSE_ZONE - flickering light [Block 4]
        // moveSpeed = cycle period (s), isActive = true during the lit phase
        if (block.type == BlockType::LIGHT_PULSE_ZONE) {
            block.timer += deltaTime;
            if (block.timer >= block.moveSpeed) {
                block.timer -= block.moveSpeed;
            }
            block.isActive = (block.timer < block.moveSpeed * 0.5f); // first half = lit
        }

        // PENDULUM_BLADE - swinging pendulums (Block 3)
        if (block.type == BlockType::PENDULUM_BLADE) {
            // Update the swing angle via a sine wave
            block.swingAngle = sin(block.timer * block.swingSpeed) * block.swingRange;

            // Advance the timer
            block.timer += deltaTime;

            // Pendulum length = distance from the pivot point to the starting position
            float length = VSize(VSub(block.pivotPoint, block.originalPos));

            // Blade position derived from the swing angle
            block.pos.x = block.pivotPoint.x + sin(block.swingAngle) * length;
            block.pos.y = block.pivotPoint.y - cos(block.swingAngle) * length;
            block.pos.z = block.pivotPoint.z;
        }
    }
}

bool Level::CheckTeleportTrigger(VECTOR playerPos, VECTOR playerSize, VECTOR& teleportTarget) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::TELEPORT) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {

                // linkId=0: wrong choice — send the player back to the level spawn
                if (block.linkId == 0) {
                    teleportTarget = VGet(playerSpawn.x + 0.5f, playerSpawn.y + 2.0f, playerSpawn.z + 0.5f);
                    return true;
                }

                for (const auto& otherBlock : blocks) {
                    if (otherBlock.type == BlockType::TELEPORT &&
                        otherBlock.linkId == block.linkId &&
                        &otherBlock != &block) {

                        teleportTarget = VGet(otherBlock.pos.x + otherBlock.size.x / 2,
                            otherBlock.pos.y + otherBlock.size.y + 1,
                            otherBlock.pos.z + otherBlock.size.z / 2);
                        return true;
                    }
                }

                // Level 5's "fake" teleports (level_block1.cpp:48-50) are the one place
                // where an unpaired linkId 1/2/3 intentionally sends the player to a
                // fixed point elsewhere in the level (part of the mechanic) instead of
                // back to spawn. Gated on levelId so that if a future level happens to
                // reuse linkId 1/2/3 without a matching pair, it won't silently jump to
                // these coordinates — it'll fall through to the spawn fallback below,
                // same as linkId == 0.
                if (levelId == 5) {
                    if (block.linkId == 1) { teleportTarget = VGet(25, 7, 0); return true; }
                    if (block.linkId == 2) { teleportTarget = VGet(5, 15, 0); return true; }
                    if (block.linkId == 3) { teleportTarget = VGet(20, 3, 3); return true; }
                }
                teleportTarget = VGet(playerSpawn.x + 0.5f, playerSpawn.y + 2.0f, playerSpawn.z + 0.5f);
                return true;
            }
        }
    }
    return false;
}

void Level::ActivateButton(VECTOR playerPos, VECTOR playerSize, bool keyPressed) {
    for (auto& block : blocks) {
        if (block.type == BlockType::BUTTON) {
            float horizDist = sqrtf(
                (playerPos.x - (block.pos.x + block.size.x / 2)) * (playerPos.x - (block.pos.x + block.size.x / 2)) +
                (playerPos.z - (block.pos.z + block.size.z / 2)) * (playerPos.z - (block.pos.z + block.size.z / 2))
            );
            // Without the vertical check, a button could be "pressed" through a
            // floor/ceiling in multi-level sections — whenever the player was within
            // 3 units on XZ, even on a completely different floor.
            float vertDist = fabsf(playerPos.y - (block.pos.y + block.size.y / 2));

            if (horizDist < 3.0f && vertDist < 3.0f && keyPressed) {
                block.isActive = !block.isActive;

                if (block.linkId != 0) {
                    for (auto& otherBlock : blocks) {
                        if (otherBlock.linkId == block.linkId && otherBlock.type == BlockType::DISAPPEARING) {
                            otherBlock.isActive = block.isActive;
                        }
                        if (otherBlock.linkId == block.linkId && otherBlock.type == BlockType::INVISIBLE_WALL) {
                            otherBlock.isActive = block.isActive;
                        }
                    }
                }
            }
        }
    }
}

bool Level::IsAnyLightPulseZoneActive() const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::LIGHT_PULSE_ZONE && block.isActive)
            return true;
    }
    return false;
}

ModelID Level::BlockTypeToModelID(BlockType type) {
    // Explicit mapping instead of relying on the enums lining up positionally —
    // BlockType::LIGHT_PULSE_ZONE(14) and ModelID::PLAYER(14) already diverged once.
    switch (type) {
        case BlockType::PLATFORM:            return ModelID::PLATFORM;
        case BlockType::TRIGGER:             return ModelID::TRIGGER;
        case BlockType::INVISIBLE_WALL:      return ModelID::INVISIBLE_WALL;
        case BlockType::FAKE_PLATFORM:       return ModelID::FAKE_PLATFORM;
        case BlockType::SPIKES:              return ModelID::SPIKES;
        case BlockType::DISAPPEARING:        return ModelID::DISAPPEARING;
        case BlockType::MOVING:              return ModelID::MOVING;
        case BlockType::BUTTON:              return ModelID::BUTTON;
        case BlockType::TELEPORT:            return ModelID::TELEPORT;
        case BlockType::RETRACTABLE_SPIKES:  return ModelID::RETRACTABLE_SPIKES;
        case BlockType::CRUMBLING:           return ModelID::CRUMBLING;
        case BlockType::FAKE_SPIKES:         return ModelID::FAKE_SPIKES;
        case BlockType::GRAVITY_ZONE:        return ModelID::GRAVITY_ZONE;
        case BlockType::PENDULUM_BLADE:      return ModelID::PENDULUM_BLADE;
        case BlockType::ICE_PLATFORM:        return ModelID::ICE_PLATFORM;
        case BlockType::BOUNCE_PAD:          return ModelID::BOUNCE_PAD;
        case BlockType::LIGHT_PULSE_ZONE:    return ModelID::COUNT; // no model, the zone is invisible
        default:                             return ModelID::COUNT;
    }
}

void Level::InitializeModels() {
    ModelManager& modelMgr = ModelManager::GetInstance();

    for (auto& block : blocks) {
        // Resolve the ModelID for this BlockType
        ModelID modelId = BlockTypeToModelID(block.type);

        // If a model is loaded for this block type
        if (modelMgr.IsModelLoaded(modelId)) {
            // Duplicate the model for this block
            block.modelHandle = modelMgr.DuplicateModel(modelId);
            block.useModel = true;

            // Scale: divide by the model's native size (~2.0 for the Quaternius pack)
            // MV1SetScale takes a multiplier, not an absolute size
            const float NATIVE = 2.0f;
            block.modelScale = VGet(
                block.size.x / NATIVE,
                block.size.y / NATIVE,
                block.size.z / NATIVE
            );

            // Flag (TRIGGER): face the player
            if (block.type == BlockType::TRIGGER)
                block.rotation = VGet(0, DX_PI_F * 0.5f, 0);
            // Pendulum cylinder: the model stands vertically, lay it along Z (90° around X)
            if (block.type == BlockType::PENDULUM_BLADE)
                block.rotation = VGet(DX_PI_F * 0.5f, 0, 0);
        }
        else {
            // No model loaded - fall back to primitives
            block.modelHandle = -1;
            block.useModel = false;
        }
    }
}

void Level::DrawSkybox(VECTOR cameraPos) const {
    ModelManager& modelMgr = ModelManager::GetInstance();

    // Draw the skybox for the current level block
    if (modelMgr.IsSkyboxLoaded(blockId)) {
        modelMgr.DrawSkybox(blockId, cameraPos);
    }
}

// Block 3: gravity zone checks
float Level::CheckGravityZone(VECTOR playerPos, VECTOR playerSize) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::GRAVITY_ZONE) {
            // Check whether the player is inside the zone
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return block.gravityMultiplier; // Return the gravity multiplier
            }
        }
    }
    return 1.0f; // Normal gravity by default
}

float Level::CheckIcePlatform(VECTOR playerPos, VECTOR playerSize) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::ICE_PLATFORM) {
            float blockTop = block.pos.y + block.size.y;
            // Player is standing on ice: feet (playerPos.y) at the block's top face + X/Z overlap
            if (playerPos.y >= blockTop - 0.15f && playerPos.y <= blockTop + 0.15f &&
                playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return block.frictionMultiplier;
            }
        }
    }
    return 1.0f; // Not on ice — normal control
}
