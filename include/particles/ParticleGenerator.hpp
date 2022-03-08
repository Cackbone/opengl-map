#pragma once

#include <vector>

#include "particles/Particle.hpp"

class ParticleGenerator {
    public:
        ParticleGenerator();

        void update(float dt);
        void render();

        void emit(glm::vec3 pos);

    private:
        std::vector<Particle> m_particlePool;
        size_t m_poolIndex = 999;
        int m_poolSize = 1000;
};