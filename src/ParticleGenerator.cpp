#include "ParticleGenerator.hpp"

BEGIN_VISUALIZER_NAMESPACE

ParticleGenerator::ParticleGenerator() : m_particlePool(m_poolSize) {
    //std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

ParticleGenerator::~ParticleGenerator() {
    glDeleteBuffers(1, &m_quadVBO);
    glDeleteBuffers(1, &m_quadIBO);
    glDeleteVertexArrays(1, &m_quadVAO);
    glDeleteProgram(m_ShaderProgram);
}

void ParticleGenerator::init() {
    std::vector<long> indices(m_poolSize * 6);
    long offset = 0l;
    for (int i = 0; i < indices.size(); i+= 6) {
        indices[i] = offset;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;

        offset += 4l;
    }
    m_nbIndexes = static_cast<int>(indices.size());

    glCreateVertexArrays(1, &m_quadVAO);
    glBindVertexArray(m_quadVAO);

    glCreateBuffers(1, &m_quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexDataPosition3fColor3f) * m_poolSize * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataPosition3fColor3f), (const void *)offsetof(VertexDataPosition3fColor3f, position));
    glEnableVertexAttribArray(1);
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

void ParticleGenerator::update(float dt) {
    m_lastEmission += dt;
    auto b = m_lastEmission >= (1 / m_emitRate);
    (void)b;
    if (m_lastEmission >= (1 / m_emitRate)) {
        emit(glm::vec3(0.0f, static_cast<float>(m_distCoord(m_seed)), 0.0f));
        m_lastEmission = 0;
    }
    for (auto& particle : m_particlePool) {
        if (!particle.active)
            continue;
        if (particle.live <= 0) {
            m_activeParticle--;
            particle.active = false;
            continue;
        }
        particle.live -= dt;
        particle.position += particle.velocity * dt;
    }
}

std::array<VertexDataPosition3fColor3f, 4> ParticleGenerator::generateQuad(Particle &part) {
    glm::vec3 color = glm::mix(part.colorEnd, part.colorStart, part.live);
    std::array<VertexDataPosition3fColor3f, 4> quad;
    // left down vertice
    quad[0] = { .position = glm::vec3(part.position.x, part.position.y, part.position.z), .color = color };
    // right down vertice
    quad[1] = { .position = glm::vec3(part.position.x + part.size, part.position.y, part.position.z), .color = color };
    // right up vertice
    quad[2] = { .position = glm::vec3(part.position.x + part.size, part.position.y + part.size, part.position.z), .color = color };
    // left up vertice
    quad[3] = { .position = glm::vec3(part.position.x, part.position.y + part.size, part.position.z), .color = color };
    return quad;
}

void ParticleGenerator::render(float dt) {
    update(dt);
    std::vector<VertexDataPosition3fColor3f> vertices;
    for (auto& particle : m_particlePool) {
        if (!particle.active)
            continue;
        auto quad = generateQuad(particle);
        vertices.insert(vertices.end(), { quad[0], quad[1], quad[2], quad[3] });
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glClearBufferData(GL_ARRAY_BUFFER, GL_R32F, GL_RED, GL_FLOAT, 0);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexDataPosition3fColor3f) * vertices.size(), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(m_ShaderProgram);
    glBindVertexArray(m_quadVAO);
    glDrawElements(GL_TRIANGLES, m_nbIndexes, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
}

void ParticleGenerator::emit(glm::vec3 pos) {
    m_activeParticle++;
    Particle& particle = m_particlePool[m_poolIndex];

    particle.position = pos;
    particle.colorStart = glm::vec3(0.882f, 0.749f, 0.572f);
    particle.colorEnd = glm::vec3(0.0f);
    particle.live = static_cast<float>(m_distLife(m_seed));
    particle.active = true;
    particle.velocity = glm::vec3(10.0f, 0.0f, 0.0f);
    particle.size = static_cast<float>(m_distSize(m_seed));

    m_poolIndex = m_poolIndex == 0 ? m_poolSize - 1 : m_poolIndex - 1;
}

END_VISUALIZER_NAMESPACE