#pragma once
#include "DxLib.h"
#include <vector>

struct Particle {
    VECTOR       pos, vel;
    float        life, maxLife;
    unsigned int color;
};

// Pooled particle burst effect used for the level-complete celebration.
class ParticleSystem {
public:
    ParticleSystem();

    void SpawnBurst(VECTOR origin, int count);
    void Update(float deltaTime);
    void Draw() const;
    void Clear();
    bool IsEmpty() const { return particles.empty(); }

private:
    std::vector<Particle> particles;
};
