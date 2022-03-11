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
        ~ParticleGenerator();

        void init();
        void update(float dt);
        void render();

        void emit(glm::vec3 pos);

    private:
        std::array<VertexDataPosition3fColor3f, 4> generateQuad(glm::vec3 pos, glm::vec3 color);

        size_t m_poolIndex = 99;
        int m_poolSize = 100;
        std::vector<Particle> m_particlePool;
        glm::vec3 m_position = glm::vec3(0.0f);

        GLuint m_quadVAO = 0;
        GLuint m_quadVBO = 0;
        GLuint m_quadIBO = 0;
        GLuint m_ShaderProgram = 0;

        std::vector<long> m_indices;
        int m_nbIndexes = 0;
};

END_VISUALIZER_NAMESPACE