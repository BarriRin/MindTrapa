#include "ParticleSystem.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>

ParticleSystem::ParticleSystem() {
    particles.reserve(300);
}

void ParticleSystem::SpawnBurst(VECTOR origin, int count) {
    static const unsigned int palette[] = {
        GetColor(255, 210,  50),
        GetColor(255, 255, 255),
        GetColor( 80, 220, 255),
        GetColor(255, 100, 200),
        GetColor(100, 255, 150),
        GetColor(255, 160,  50),
    };

    for (int i = 0; i < count; i++) {
        Particle p;
        float az   = (rand() % 6284) * 0.001f;
        float elev = ((rand() % 100) - 20) * 0.02f;
        float spd  = 3.0f + (rand() % 100) * 0.06f;
        p.vel = VGet(cosf(elev) * cosf(az) * spd,
                     sinf(elev) * spd + 2.5f,
                     cosf(elev) * sinf(az) * spd);
        p.pos = VAdd(origin, VGet((rand() % 100 - 50) * 0.01f,
                                   (rand() % 80)        * 0.01f,
                                   (rand() % 100 - 50) * 0.01f));
        p.maxLife = 1.4f + (rand() % 100) * 0.012f;
        p.life    = p.maxLife;
        p.color   = palette[rand() % 6];
        particles.push_back(p);
    }
}

void ParticleSystem::Update(float deltaTime) {
    for (auto& p : particles) {
        p.vel.y -= 8.0f * deltaTime;
        p.pos    = VAdd(p.pos, VScale(p.vel, deltaTime));
        p.life  -= deltaTime;
    }
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return p.life <= 0.0f; }),
        particles.end());
}

void ParticleSystem::Draw() const {
    for (const auto& p : particles) {
        VECTOR sp = ConvWorldPosToScreenPos(p.pos);
        if (sp.z <= 0.0f || sp.z >= 1.0f) continue;
        float lifeRatio = p.life / p.maxLife;
        int   a = (int)(lifeRatio * 220.0f);
        if (a < 5) continue;
        float r = 7.0f / (sp.z + 0.05f);
        if (r <  2.0f) r =  2.0f;
        if (r > 14.0f) r = 14.0f;
        SetDrawBlendMode(DX_BLENDMODE_ADD, a);
        DrawCircle((int)sp.x, (int)sp.y, (int)r, p.color, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ADD, a / 4);
        DrawCircle((int)sp.x, (int)sp.y, (int)(r * 2.8f), p.color, TRUE);
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ParticleSystem::Clear() {
    particles.clear();
}
