#pragma once

#include <vector>
#include <GL/glew.h>

#include "Particle.hpp"
#include "utils.hpp"
#include "shader/Shader.hpp"
#include "shader/ShaderLinker.hpp"

BEGIN_VISUALIZER_NAMESPACE

class ParticleGenerator {
    public:
        ParticleGenerator();

        void init();
        void update(float dt);
        void render();

        void emit(glm::vec3 pos);

    private:
        size_t m_poolIndex = 999;
        int m_poolSize = 1000;
        std::vector<Particle> m_particlePool;

        GLuint m_quadVAO = 0;
        GLuint m_quadVBO = 0;
        GLuint m_quadIBO = 0;
        GLuint m_ShaderProgram = 0;
};

END_VISUALIZER_NAMESPACE