#include "Renderer.h"
#include <random>
#include <cmath>
#include <vector>

// ── Биом-конфиги ──────────────────────────────────────────────────────────────

struct BiomeConfig {
    // Небо: два DrawCube3D — верхняя и нижняя полусферы
    unsigned int skyTopA, skyTopB;      // верх
    unsigned int skyBotA, skyBotB;      // низ

    // Звёзды
    unsigned int starColors[4];
    int          starColorCount;
    float        starSizeMin, starSizeMax;

    // Планета (DrawSphere3D)
    bool         hasPlanet;
    float        planetOffX, planetOffY, planetOffZ;  // смещение от игрока
    float        planetRadius;
    unsigned int planetColorDif;
    unsigned int planetColorSpc;

    // Второй объект (луна, кольцо и т.п.)
    bool         hasMoon;
    float        moonOffX, moonOffY, moonOffZ;
    float        moonRadius;
    unsigned int moonColor;
};

static const BiomeConfig BIOMES[5] = {

    // 0 — Deep Space (Block 1: туториал, классический космос)
    {
        GetColor( 3,  3, 18), GetColor( 8,  8, 28),   // небо верх
        GetColor( 8,  8, 28), GetColor(12,  8, 22),   // небо низ
        { GetColor(255,255,255), GetColor(200,220,255),
          GetColor(255,240,200), GetColor(180,200,255) }, 4,
        0.3f, 1.6f,
        // далёкая синеватая звезда
        true,  380.0f, 180.0f, -450.0f, 55.0f,
        GetColor(140, 160, 220), GetColor(80, 100, 180),
        // маленькая луна
        true,  420.0f, 130.0f, -490.0f, 18.0f, GetColor(180,180,190),
    },

    // 1 — Nebula / Red Giant (Block 2: красно-оранжевая туманность)
    {
        GetColor(22,  4, 12), GetColor(35,  8, 10),
        GetColor(30,  6,  8), GetColor(18,  4, 16),
        { GetColor(255,140, 80), GetColor(255,100,120),
          GetColor(220, 80, 60), GetColor(255,200,100) }, 4,
        0.4f, 2.0f,
        // красный гигант — большой и тусклый
        true, -350.0f, 220.0f, -500.0f, 90.0f,
        GetColor(200,  60,  20), GetColor(255, 120,  40),
        // спутник
        true, -290.0f, 160.0f, -520.0f, 22.0f, GetColor(140, 80, 60),
    },

    // 2 — Gas Giant (Block 3: гравитация, фиолетово-синий)
    {
        GetColor( 8,  3, 30), GetColor(15,  5, 45),
        GetColor(12,  4, 40), GetColor(20,  8, 35),
        { GetColor(180,120,255), GetColor(100,140,255),
          GetColor(220,180,255), GetColor( 80,180,220) }, 4,
        0.5f, 2.2f,
        // огромный газовый гигант
        true,  280.0f, 100.0f, -480.0f, 130.0f,
        GetColor(140,  80, 200), GetColor(100,  40, 160),
        // маленький спутник
        true,  390.0f,  60.0f, -510.0f, 24.0f, GetColor(160,140,180),
    },

    // 3 — Void (Block 4: темнота, почти чёрный)
    {
        GetColor( 2,  2,  5), GetColor( 4,  3,  8),
        GetColor( 4,  3,  8), GetColor( 3,  2,  6),
        { GetColor(220,220,240), GetColor(180,180,200),
          GetColor(255,255,255), GetColor(160,160,180) }, 3,
        0.2f, 1.0f,
        // нейтронная звезда / пульсар — маленький и яркий
        true,  300.0f, 250.0f, -460.0f, 20.0f,
        GetColor(200,220,255), GetColor(255,255,255),
        false, 0,0,0, 0, 0,
    },

    // 4 — Ice Planet (Block 5: ледяная планета, голубой)
    {
        GetColor( 2, 15, 35), GetColor( 5, 25, 55),
        GetColor( 5, 22, 50), GetColor( 8, 30, 45),
        { GetColor(200,240,255), GetColor(150,210,255),
          GetColor(255,255,255), GetColor(180,230,240) }, 4,
        0.3f, 1.8f,
        // ледяная планета — голубая
        true, -320.0f, 160.0f, -500.0f, 100.0f,
        GetColor( 80,160,220), GetColor(140,200,255),
        // ледяная луна
        true, -220.0f, 110.0f, -530.0f, 28.0f, GetColor(180,220,240),
    },
};

// ── Сферический скайбокс ──────────────────────────────────────────────────────

// Форма сферы неизменна между кадрами — меняется только положение игрока.
// Локальные смещения/нормали/UV считаются один раз и кэшируются;
// каждый кадр обновляется только позиция (VAdd), без тригонометрии и реаллокаций.
static void BuildSkyboxSphereCache(float radius, std::vector<VECTOR>& offsets, std::vector<VERTEX3D>& verts) {
    const int STACKS = 18;
    const int SLICES = 36;

    offsets.clear();
    verts.clear();
    offsets.reserve(STACKS * SLICES * 6);
    verts.reserve(STACKS * SLICES * 6);

    for (int ti = 0; ti < STACKS; ti++) {
        for (int si = 0; si < SLICES; si++) {
            float phi0   = (float)si       / SLICES * DX_PI_F * 2.0f;
            float phi1   = (float)(si + 1) / SLICES * DX_PI_F * 2.0f;
            float theta0 = (float)ti       / STACKS * DX_PI_F;
            float theta1 = (float)(ti + 1) / STACKS * DX_PI_F;

            float u0 = (float)si       / SLICES;
            float u1 = (float)(si + 1) / SLICES;
            float v0 = (float)ti       / STACKS;
            float v1 = (float)(ti + 1) / STACKS;

            auto makeV = [&](float phi, float theta, float u, float v) -> VERTEX3D {
                float x = radius * sinf(theta) * sinf(phi);
                float y = radius * cosf(theta);
                float z = radius * sinf(theta) * cosf(phi);
                VERTEX3D vert;
                vert.pos  = VGet(x, y, z);  // перезаписывается каждый кадр в DrawSkyboxSphere
                vert.norm = VGet(-sinf(theta)*sinf(phi), -cosf(theta), -sinf(theta)*cosf(phi));
                vert.dif.r = 255; vert.dif.g = 255; vert.dif.b = 255; vert.dif.a = 255;
                vert.spc.r = 0;   vert.spc.g = 0;   vert.spc.b = 0;   vert.spc.a = 0;
                vert.u = u;  vert.v = v;
                return vert;
            };

            VERTEX3D v00 = makeV(phi0, theta0, u0, v0);
            VERTEX3D v10 = makeV(phi1, theta0, u1, v0);
            VERTEX3D v01 = makeV(phi0, theta1, u0, v1);
            VERTEX3D v11 = makeV(phi1, theta1, u1, v1);

            // Перевёрнутый winding — смотрим изнутри сферы
            // offsets пушится в том же порядке и с той же кратностью, что и verts —
            // иначе индексы разъезжаются (баг: offsets был короче verts в 1.5 раза)
            verts.push_back(v00); offsets.push_back(v00.pos);
            verts.push_back(v01); offsets.push_back(v01.pos);
            verts.push_back(v10); offsets.push_back(v10.pos);
            verts.push_back(v10); offsets.push_back(v10.pos);
            verts.push_back(v01); offsets.push_back(v01.pos);
            verts.push_back(v11); offsets.push_back(v11.pos);
        }
    }
}

void DrawSkyboxSphere(VECTOR playerPos, int graphHandle, float radius) {
    if (graphHandle == -1) {
        DrawSpaceSkybox(playerPos, 0);
        return;
    }

    static std::vector<VECTOR>   s_offsets;
    static std::vector<VERTEX3D> s_verts;
    static float                 s_cachedRadius = -1.0f;

    if (s_verts.empty() || s_cachedRadius != radius) {
        BuildSkyboxSphereCache(radius, s_offsets, s_verts);
        s_cachedRadius = radius;
    }

    for (size_t i = 0; i < s_verts.size(); i++) {
        s_verts[i].pos = VAdd(playerPos, s_offsets[i]);
    }

    SetUseZBuffer3D(FALSE);
    SetWriteZBuffer3D(FALSE);
    SetUseLighting(FALSE);
    SetUseBackCulling(FALSE);

    DrawPolygon3D(s_verts.data(), (int)s_verts.size() / 3, graphHandle, TRUE);

    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
}

// ── Рендер ────────────────────────────────────────────────────────────────────

void DrawSpaceSkybox(VECTOR playerPos, int biomeIdx) {
    if (biomeIdx < 0 || biomeIdx > 4) biomeIdx = 0;
    const BiomeConfig& b = BIOMES[biomeIdx];

    float skySize = 500.0f;
    VECTOR skyMin = VGet(playerPos.x - skySize, playerPos.y - skySize, playerPos.z - skySize);
    VECTOR skyMax = VGet(playerPos.x + skySize, playerPos.y + skySize, playerPos.z + skySize);

    SetUseZBuffer3D(FALSE);
    SetWriteZBuffer3D(FALSE);

    // Верхняя полусфера
    DrawCube3D(
        VGet(skyMin.x, playerPos.y, skyMin.z),
        VGet(skyMax.x, skyMax.y,    skyMax.z),
        b.skyTopA, b.skyTopB, TRUE
    );
    // Нижняя полусфера
    DrawCube3D(
        VGet(skyMin.x, skyMin.y,    skyMin.z),
        VGet(skyMax.x, playerPos.y, skyMax.z),
        b.skyBotA, b.skyBotB, TRUE
    );

    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
}

void DrawBiomePlanet(VECTOR playerPos, int biomeIdx) {
    if (biomeIdx < 0 || biomeIdx > 4) biomeIdx = 0;
    const BiomeConfig& b = BIOMES[biomeIdx];

    SetUseZBuffer3D(FALSE);
    SetWriteZBuffer3D(FALSE);

    if (b.hasPlanet) {
        VECTOR pPos = VAdd(playerPos,
            VGet(b.planetOffX, b.planetOffY, b.planetOffZ));
        DrawSphere3D(pPos, b.planetRadius, 24,
            b.planetColorDif, b.planetColorSpc, TRUE);

        // Атмосферное свечение — чуть больший полупрозрачный шар
        SetDrawBlendMode(DX_BLENDMODE_ADD, 35);
        DrawSphere3D(pPos, b.planetRadius * 1.12f, 16,
            b.planetColorSpc, b.planetColorSpc, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    if (b.hasMoon) {
        VECTOR mPos = VAdd(playerPos,
            VGet(b.moonOffX, b.moonOffY, b.moonOffZ));
        DrawSphere3D(mPos, b.moonRadius, 16, b.moonColor, GetColor(60,60,70), TRUE);
    }

    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
}

// ── Звёзды ────────────────────────────────────────────────────────────────────

std::vector<Star> GenerateStars(int count, float radius, int biomeIdx) {
    if (biomeIdx < 0 || biomeIdx > 4) biomeIdx = 0;
    const BiomeConfig& b = BIOMES[biomeIdx];

    std::vector<Star> stars;
    std::mt19937 gen(42 + biomeIdx * 7);
    std::uniform_real_distribution<float> axisD(-1.0f, 1.0f);
    std::uniform_real_distribution<float> phaseD(0.0f, DX_PI_F * 2.0f);
    std::uniform_real_distribution<float> jitter(0.88f, 1.0f);
    std::uniform_real_distribution<float> sizeFarD (b.starSizeMin, b.starSizeMax);
    std::uniform_real_distribution<float> sizeNearD(b.starSizeMax * 1.8f, b.starSizeMax * 3.5f);

    // Void — заметно меньше звёзд
    int farCount  = (biomeIdx == 3) ? count / 5      : count * 3 / 4;
    int nearCount = (biomeIdx == 3) ? count / 20     : count / 4;

    auto spherePoint = [&](float r) -> VECTOR {
        float x, y, z, len;
        do { x = axisD(gen); y = axisD(gen); z = axisD(gen);
             len = sqrtf(x*x + y*y + z*z); } while (len < 0.001f);
        float s = r * jitter(gen) / len;
        return VGet(x * s, y * s, z * s);
    };

    stars.reserve(farCount + nearCount);

    for (int i = 0; i < farCount; i++) {
        Star star;
        star.pos   = spherePoint(radius);
        star.size  = sizeFarD(gen);
        star.color = b.starColors[i % b.starColorCount];
        star.phase = phaseD(gen);
        star.layer = 0;
        stars.push_back(star);
    }
    for (int i = 0; i < nearCount; i++) {
        Star star;
        star.pos   = spherePoint(radius * 0.45f);
        star.size  = sizeNearD(gen);
        star.color = b.starColors[i % b.starColorCount];
        star.phase = phaseD(gen);
        star.layer = 1;
        stars.push_back(star);
    }
    return stars;
}

void DrawStars(const std::vector<Star>& stars, VECTOR playerPos) {
    float t = GetNowCount() * 0.0009f;

    for (const auto& star : stars) {
        VECTOR world  = VAdd(playerPos, star.pos);
        VECTOR screen = ConvWorldPosToScreenPos(world);

        if (screen.z <= 0.001f || screen.z > 1.0f) continue;
        int sx = (int)screen.x;
        int sy = (int)screen.y;
        if (sx < -60 || sx > 1980 || sy < -60 || sy > 1140) continue;

        // Мерцание: ближние медленнее, дальние быстрее
        float speed   = (star.layer == 1) ? 0.6f : 1.3f;
        float twinkle = 0.5f + 0.5f * sinf(t * speed + star.phase);

        int   r    = (int)(star.size * (0.65f + twinkle * 0.7f) + 0.5f);
        int   core = (int)(100 + twinkle * 155);  // 100..255

        if (r < 1) r = 1;

        // Ядро
        SetDrawBlendMode(DX_BLENDMODE_ADD, core);
        DrawCircle(sx, sy, r, star.color, TRUE);

        // Мягкое свечение вокруг
        SetDrawBlendMode(DX_BLENDMODE_ADD, core / 5);
        DrawCircle(sx, sy, r * 3, star.color, TRUE);
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// ── Туман биома ───────────────────────────────────────────────────────────────

struct BiomeFogCfg { int r, g, b; float start, end; };
static const BiomeFogCfg BIOME_FOG[5] = {
    {  4,  6, 22, 18.0f, 55.0f },  // 0 Deep Space  — синий
    { 28,  4,  8, 15.0f, 50.0f },  // 1 Nebula       — тёмно-красный
    {  8,  2, 28, 12.0f, 45.0f },  // 2 Gas Giant    — фиолетовый
    {  1,  1,  4, 25.0f, 70.0f },  // 3 Void         — почти чёрный
    {  3, 14, 28, 16.0f, 52.0f },  // 4 Ice Planet   — тёмный голубой
};

void ApplyBiomeFog(int biomeIdx) {
    if (biomeIdx < 0 || biomeIdx > 4) biomeIdx = 0;
    const BiomeFogCfg& f = BIOME_FOG[biomeIdx];
    SetFogEnable(TRUE);
    SetFogColor(f.r, f.g, f.b);
    SetFogStartEnd(f.start, f.end);
}

void DisableFog() {
    SetFogEnable(FALSE);
}
