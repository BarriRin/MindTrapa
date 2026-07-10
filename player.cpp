#include "Player.h"
#include "Camera.h"
#include "AudioManager.h"
#include "level.h"
#include <cmath>
#include <algorithm>

Player::Player()
    : pos(VGet(0,0,0)), vel(VGet(0,0,0)), horizVel(VGet(0,0,0))
    , size(VGet(1,1,1))
    , onGround(false), wasOnGround(false), dead(false)
    , facingAngle(0.0f), iceFriction(1.0f), savedPlatVel(VGet(0,0,0))
    , groundedStreak(0)
    , modelHandle(-1), animAttach(-1), animTime(0.0f), currentAnim(-1)
    , animIdle(-1), animRun(-1), animJump(-1), animFall(-1)
    , animDeath(-1), animWave(-1), animYes(-1)
    , jumpPressed(false), activatePressed(false)
{}

Player::~Player() {
    ReleaseModel();
}

void Player::ReleaseModel() {
    if (modelHandle != -1) {
        MV1DeleteModel(modelHandle);
        modelHandle = -1;
    }
}

bool Player::LoadModel(const wchar_t* path) {
    modelHandle = MV1LoadModel(path);
    if (modelHandle == -1) return false;

    animIdle  = FindAnim(L"Idle");
    animRun   = FindAnim(L"Run");
    animJump  = FindAnim(L"Jump");
    animFall  = FindAnim(L"Jump_Idle");
    animDeath = FindAnim(L"Death");
    if (animDeath == -1) animDeath = FindAnim(L"Die");
    if (animDeath == -1) animDeath = FindAnim(L"Defeat");
    animWave  = FindAnim(L"Wave");
    animYes   = FindAnim(L"Yes");

#ifdef _DEBUG
    // Debug: список анимаций + какие индексы реально резолвятся и с каким total time
    // (диагностика "модель подвисает в воздухе" — total<=0 у animJump/animFall
    // означает, что ApplyAnimation молча не обновляет позу все время в воздухе)
    int animCount = MV1GetAnimNum(modelHandle);
    FILE* f = nullptr;
    fopen_s(&f, "AnimList.txt", "w");
    if (f) {
        fprintf(f, "Character.mv1 — %d animations:\n", animCount);
        for (int i = 0; i < animCount; i++)
            fprintf(f, "  [%d] %ls  total=%.3f\n", i, MV1GetAnimName(modelHandle, i), MV1GetAnimTotalTime(modelHandle, i));
        fprintf(f, "\nRESOLVED:\n");
        fprintf(f, "  animIdle=%d animRun=%d animJump=%d animFall=%d animDeath=%d animWave=%d animYes=%d\n",
            animIdle, animRun, animJump, animFall, animDeath, animWave, animYes);
        fclose(f);
    }
#endif

    return true;
}

void Player::Reset(VECTOR spawnPos) {
    pos       = spawnPos;
    vel       = VGet(0, 0, 0);
    horizVel  = VGet(0, 0, 0);
    onGround  = false;
    wasOnGround = false;
    dead      = false;
    iceFriction = 1.0f;
    savedPlatVel = VGet(0, 0, 0);
    groundedStreak = 0;
    if (modelHandle != -1 && animAttach != -1) {
        MV1DetachAnim(modelHandle, animAttach);
        animAttach   = -1;
        currentAnim  = -1;
    }
}

bool Player::Update(float dt, const Camera& camera, Level* level, AudioManager& audio, int soundVol) {
    dead = false;
    wasOnGround = onGround;

    // === ВВОД ===
    float moveSpeed = SPEED * (dt * 60.0f);
    float ay = camera.GetAngleY();
    VECTOR forward = VGet( sinf(ay), 0,  cosf(ay));
    VECTOR right   = VGet( cosf(ay), 0, -sinf(ay));
    VECTOR move    = VGet(0, 0, 0);

    if (CheckHitKey(KEY_INPUT_W)) move = VAdd(move, VScale(forward, -moveSpeed));
    if (CheckHitKey(KEY_INPUT_S)) move = VAdd(move, VScale(forward,  moveSpeed));
    if (CheckHitKey(KEY_INPUT_A)) move = VAdd(move, VScale(right,    moveSpeed));
    if (CheckHitKey(KEY_INPUT_D)) move = VAdd(move, VScale(right,   -moveSpeed));

    // Кнопка активации E (единичный импульс)
    bool activateKeyPressed = false;
    if (CheckHitKey(KEY_INPUT_E)) {
        if (!activatePressed) activateKeyPressed = true;
        activatePressed = true;
    } else {
        activatePressed = false;
    }

    // === ЗОНА ЛЬДА (Block 5) ===
    iceFriction = level ? level->CheckIcePlatform(pos, size) : 1.0f;

    // === ПРЫЖОК ===
    if (CheckHitKey(KEY_INPUT_SPACE)) {
        if (!jumpPressed && onGround && iceFriction >= 1.0f) {
            vel.y    = JUMP_POWER;
            onGround = false;
            audio.PlayJump(soundVol);
        }
        jumpPressed = true;
    } else {
        jumpPressed = false;
    }

    // === ГРАВИТАЦИЯ И ЗОНЫ ===
    float gravMult = level ? level->CheckGravityZone(pos, size) : 1.0f;
    vel.y += GRAVITY * gravMult * (dt * 60.0f);
    // Симметрично: gravityMultiplier бывает отрицательным (reverse-зоны), там разгон идёт вверх
    if (vel.y < MAX_FALL_SPEED)  vel.y = MAX_FALL_SPEED;
    if (vel.y > -MAX_FALL_SPEED) vel.y = -MAX_FALL_SPEED;

    // === ИНЕРЦИЯ ЛЬДА ===
    if (iceFriction < 1.0f) {
        bool hasInput = (move.x != 0.0f || move.z != 0.0f);
        if (hasInput) {
            const float blend = 0.04f;
            horizVel.x = horizVel.x * (1.f - blend) + move.x * blend;
            horizVel.z = horizVel.z * (1.f - blend) + move.z * blend;
        } else {
            float decay = powf(iceFriction, dt * 60.0f);
            horizVel.x *= decay;
            horizVel.z *= decay;
        }
    } else {
        horizVel = move;
    }

    // === КОЛЛИЗИЯ ===
    VECTOR newPos = VAdd(pos, VAdd(horizVel, VGet(0, vel.y * (dt * 60.0f), 0)));

    // Тянем игрока вниз за опускающейся платформой (иначе AABB не засечёт перекрытие)
    if (savedPlatVel.y < 0.0f)
        newPos.y += savedPlatVel.y;

    if (level) {
        VECTOR platVel = VGet(0, 0, 0);
        bool   justBounced = false;
        level->CheckCollision(pos, size, newPos, vel, onGround, platVel, gravMult < 0.0f, &justBounced);
        if (justBounced) audio.PlayBounce(soundVol);
        newPos.x += platVel.x;
        newPos.z += platVel.z;
        // platVel.y пропускаем — CheckCollision уже снэпнул игрока на верх платформы
        savedPlatVel = onGround ? platVel : VGet(0, 0, 0);

        // Телепорт — обрабатывается снаружи (cooldown)
        // Кнопка — здесь вызываем, по уже обновлённой позиции (согласовано с
        // CheckTeleportTrigger в main.cpp, который тоже использует позицию после коллизии)
        level->ActivateButton(newPos, size, activateKeyPressed);
    }

    pos = newPos;

    // Приземление
    if (!wasOnGround && onGround) audio.PlayLand(soundVol);

    // Упал за пределы / смерть от шипов
    if (level && level->CheckDeadlyTrigger(pos, size)) {
        dead = true;
    }
    if (pos.y < DEATH_FALL_Y) {
        dead = true;
    }

    if (dead) {
        vel      = VGet(0, 0, 0);
        horizVel = VGet(0, 0, 0);
        audio.PlayDeath(soundVol);
        SwitchAnim(animDeath);
        return true; // умер
    }

    // === АНИМАЦИЯ ===
    bool isMovingH = (fabsf(move.x) > 0.001f || fabsf(move.z) > 0.001f);
    if (isMovingH) facingAngle = atan2f(move.x, move.z);

    // onGround у краёв платформ может мигать true/false покадрово (AABB-контакт на грани) —
    // не даём одиночному "заземлению" сразу переключать на Idle/Run, иначе SwitchAnim()
    // сбрасывает animTime каждый такой кадр и поза Jump/Fall визуально подвисает
    groundedStreak = onGround ? groundedStreak + 1 : 0;
    bool animGrounded = groundedStreak >= 2;

    int targetAnim;
    if (!animGrounded) targetAnim = (vel.y > 0.0f) ? animJump : animFall;
    else if (isMovingH) targetAnim = animRun;
    else                targetAnim = animIdle;

    if (targetAnim != currentAnim) SwitchAnim(targetAnim);
    ApplyAnimation(dt, true);

    // Поворот лицом к направлению движения
    if (modelHandle != -1) {
        VECTOR modelPos = VGet(pos.x + size.x * 0.5f, pos.y, pos.z + size.z * 0.5f);
        MV1SetPosition(modelHandle, modelPos);
        MV1SetScale   (modelHandle, VGet(0.006f, 0.006f, 0.006f));
        MV1SetRotationXYZ(modelHandle, VGet(0.0f, facingAngle + DX_PI_F, 0.0f));
    }

    return false; // жив
}

void Player::Draw() const {
    if (modelHandle != -1) {
        MV1DrawModel(modelHandle);
    } else {
        DrawCube3D(pos, VAdd(pos, size),
            GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
    }
}

void Player::DrawFacingCamera(VECTOR camPos) const {
    VECTOR center = VGet(pos.x + size.x * 0.5f, pos.y, pos.z + size.z * 0.5f);
    float  angle  = atan2f(camPos.x - center.x, camPos.z - center.z);

    if (modelHandle != -1) {
        MV1SetPosition    (modelHandle, center);
        MV1SetScale       (modelHandle, VGet(0.006f, 0.006f, 0.006f));
        MV1SetRotationXYZ (modelHandle, VGet(0.0f, angle + DX_PI_F, 0.0f));
        MV1DrawModel      (modelHandle);
    } else {
        DrawCube3D(pos, VAdd(pos, size),
            GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
    }
}

void Player::DrawShadow(Level* level) const {
    if (!level) return;

    bool  foundPlatform = false;
    float shadowY       = 0.0f;
    float closestDist   = 1000.0f;

    for (const auto& block : level->GetBlocks()) {
        if (block.type != BlockType::PLATFORM &&
            block.type != BlockType::TRIGGER  &&
            !(block.type == BlockType::DISAPPEARING && block.isActive))
            continue;

        if (pos.x < block.pos.x - 2.0f || pos.x > block.pos.x + block.size.x + 2.0f) continue;
        if (pos.z < block.pos.z - 2.0f || pos.z > block.pos.z + block.size.z + 2.0f) continue;

        float top = block.pos.y + block.size.y;
        if (top < pos.y) {
            float dist = pos.y - top;
            if (dist < closestDist) { closestDist = dist; shadowY = top + 0.01f; foundPlatform = true; }
        }
    }

    if (!foundPlatform) return;

    float shadowSize = 0.8f + closestDist * 0.05f;
    if (shadowSize > 2.0f) shadowSize = 2.0f;
    int   alpha = (int)(100.0f - closestDist * 5.0f);
    if (alpha < 20)  alpha = 20;
    if (alpha > 100) alpha = 100;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    DrawCube3D(
        VGet(pos.x - shadowSize * 0.5f, shadowY,         pos.z - shadowSize * 0.5f),
        VGet(pos.x + shadowSize * 0.5f, shadowY + 0.05f, pos.z + shadowSize * 0.5f),
        GetColor(0,0,0), GetColor(0,0,0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Player::StartCelebration() {
    SwitchAnim(animYes);
}

void Player::PlayWave() {
    SwitchAnim(animWave);
}

void Player::UpdateAnimOnly(float dt, bool looping) {
    ApplyAnimation(dt, looping);
}

float Player::GetFacingToward(VECTOR target) const {
    VECTOR center = VGet(pos.x + size.x * 0.5f, pos.y, pos.z + size.z * 0.5f);
    return atan2f(target.x - center.x, target.z - center.z);
}

// ── Private ──────────────────────────────────────────────────────────────────

int Player::FindAnim(const wchar_t* substr) const {
    if (modelHandle == -1) return -1;
    int n = MV1GetAnimNum(modelHandle);
    for (int i = 0; i < n; i++)
        if (wcsstr(MV1GetAnimName(modelHandle, i), substr)) return i;
    return -1;
}

void Player::SwitchAnim(int animIdx) {
    if (modelHandle == -1) return;
    if (animAttach != -1) MV1DetachAnim(modelHandle, animAttach);
    animAttach  = MV1AttachAnim(modelHandle, animIdx, -1, FALSE);
    animTime    = 0.0f;
    currentAnim = animIdx;
}

void Player::ApplyAnimation(float dt, bool looping) {
    if (modelHandle == -1 || animAttach == -1) return;
    float total = MV1GetAttachAnimTotalTime(modelHandle, animAttach);
    if (total <= 0.0f) return;
    animTime += dt * ANIM_FRAMERATE;
    if (looping) {
        animTime = fmodf(animTime, total);
    } else {
        if (animTime > total) animTime = total;
    }
    MV1SetAttachAnimTime(modelHandle, animAttach, animTime);
}
