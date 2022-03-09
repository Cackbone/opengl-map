#include "particles/ParticleGenerator.hpp"

ParticleGenerator::ParticleGenerator() : m_particlePool(m_poolSize) {
    float vertices[] = {
         -0.5f, -0.5f, 0.0f,
          0.5f, -0.5f, 0.0f,
          0.5f,  0.5f, 0.0f,
         -0.5f,  0.5f, 0.0f
    };
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
    glCreateVertexArrays(1, &m_quadVAO);
    glBindVertexArray(m_quadVAO);

    glCreateBuffers(1, &m_quadVBO);
    //glNamedBufferStorage(m_quadVBO, sizeof(VertexDataPosition3fColor3f) * vertices.size(), vertices.data(), 0);
}

void ParticleGenerator::render() {

}

void ParticleGenerator::emit(glm::vec3 pos) {
    Particle& particle = m_particlePool[m_poolIndex];

    particle.position = pos;
    particle.live = 1;
    particle.active = true;

    m_poolIndex = --m_poolIndex % m_particlePool.size();
}