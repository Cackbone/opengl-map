#pragma once

#include <vector>
#include <GL/glew.h>

#include "particles/Particle.hpp"

class ParticleGenerator {
    public:
        ParticleGenerator();

        void init();
        void update(float dt);
        void render();

        void emit(glm::vec3 pos);

    private:
        std::vector<Particle> m_particlePool;
        size_t m_poolIndex = 999;
        int m_poolSize = 1000;

        GLuint m_quadVAO = 0;
        GLuint m_quadVBO = 0;
};