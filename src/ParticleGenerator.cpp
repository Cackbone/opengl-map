#include "ParticleGenerator.hpp"

BEGIN_VISUALIZER_NAMESPACE

ParticleGenerator::ParticleGenerator() : m_particlePool(m_poolSize) {
}

void ParticleGenerator::update(float dt) {
    for (auto& particle : m_particlePool) {
        if (!particle.active)
            continue;
        if (particle.live <= 0) {
            particle.active = false;
            continue;
        }
        particle.live -= dt;
        particle.position += particle.velocity * dt;
    }
}

void ParticleGenerator::init() {
    std::vector<VertexDataPosition3fColor3f> vertices({
        { {-0.5f, -0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f } },
        { {0.5f, -0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f } },
        { {0.5f,  0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f } },
        { {-0.5f,  0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f } }
    });

    std::vector<long> indices = {
        0, 1, 2, 2, 3, 0,
    };

    glCreateVertexArrays(1, &m_quadVAO);
    glBindVertexArray(m_quadVAO);

    glCreateBuffers(1, &m_quadVBO);
    //glNamedBufferStorage(m_quadVBO, sizeof(VertexDataPosition3fColor3f) * vertices.size(), vertices.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexDataPosition3fColor3f) * 4, nullptr, GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);



    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataPosition3fColor3f), nullptr);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataPosition3fColor3f), (const void *)offsetof(VertexDataPosition3fColor3f, position));
    glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataPosition3fColor3f), reinterpret_cast<GLvoid*>(sizeof(glm::vec3)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataPosition3fColor3f), (const void*)offsetof(VertexDataPosition3fColor3f, color));
    glCreateBuffers(1, &m_quadIBO);
    glNamedBufferStorage(m_quadIBO, sizeof(long) * indices.size(), indices.data(), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBO);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    Shader vShader(GL_VERTEX_SHADER, "../../src/shader/ParticleShader.vs");
    Shader fShader(GL_FRAGMENT_SHADER, "../../src/shader/ParticleShader.fs");

    ShaderLinker shaderLinker({
            vShader,
            fShader
        });

    m_ShaderProgram = shaderLinker.id();

    glDetachShader(m_ShaderProgram, vShader.id());
    glDetachShader(m_ShaderProgram, fShader.id());
}

void ParticleGenerator::render() {
    std::vector<VertexDataPosition3fColor3f> vertices({
        { {-0.5f, -0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f } },
        { {0.5f, -0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f } },
        { {0.5f,  0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f } },
        { {-0.5f,  0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f } }
        });
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexDataPosition3fColor3f) * vertices.size(), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(m_ShaderProgram);
    glBindVertexArray(m_quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
}

void ParticleGenerator::emit(glm::vec3 pos) {
    Particle& particle = m_particlePool[m_poolIndex];

    particle.position = pos;
    particle.live = 1;
    particle.active = true;

    m_poolIndex = --m_poolIndex % m_particlePool.size();
}

END_VISUALIZER_NAMESPACE