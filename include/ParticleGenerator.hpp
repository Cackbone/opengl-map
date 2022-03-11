#pragma once

#include <vector>
#include <random>
#include <stdlib.h>
#include <GL/glew.h>

#include "Particle.hpp"
#include "utils.hpp"
#include "shader/Shader.hpp"
#include "shader/ShaderLinker.hpp"

BEGIN_VISUALIZER_NAMESPACE

class ParticleGenerator {
    public:
        ParticleGenerator(glm::vec3 pos = glm::vec3(0.0f));
        ~ParticleGenerator();

        void init();
        void update(float dt); // dt in seconds
        void render(float dt); // dt in seconds

        void emit(glm::vec3 pos);

    private:
        std::array<VertexDataPosition3fColor3f, 4> generateQuad(Particle& part);

        // particle pool
        unsigned int m_poolSize = 100;
        size_t m_poolIndex = m_poolSize - 1;
        std::vector<Particle> m_particlePool;
        std::vector<long> m_indices;
        int m_nbIndexes = 0;
        unsigned int m_activeParticle = 0;

        // generator attributs
        glm::vec3 m_position = glm::vec3(0.0f);
        float m_emitRate = 30; // number of particle per seconds
        float m_lastEmission = 0;

        // buffer data
        GLuint m_quadVAO = 0;
        GLuint m_quadVBO = 0;
        GLuint m_quadIBO = 0;
        GLuint m_ShaderProgram = 0;

        // random
        std::random_device m_seed;
        std::mt19937 m_coordGen = std::mt19937(m_seed()); // generator for particle coordinate
        std::uniform_real_distribution<> m_distCoord = std::uniform_real_distribution<>(-0.5, 0.5);
        std::uniform_real_distribution<> m_distLife = std::uniform_real_distribution<>(1, 1.5);
        std::uniform_real_distribution<> m_distSize = std::uniform_real_distribution<>(0.02, 0.08);
};

END_VISUALIZER_NAMESPACE