#include "ParticleGenerator.hpp"

BEGIN_VISUALIZER_NAMESPACE

ParticleGenerator::ParticleGenerator() : m_particlePool(m_poolSize) {
}

ParticleGenerator::~ParticleGenerator() {
    glDeleteBuffers(1, &m_quadVBO);
    glDeleteBuffers(1, &m_quadIBO);
    glDeleteVertexArrays(1, &m_quadVAO);
    glDeleteProgram(m_ShaderProgram);
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
    //std::vector<VertexDataPosition3fColor3f> vertices({
    //    { {-0.5f, -0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f } },
    //    { {0.5f, -0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f } },
    //    { {0.5f,  0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f } },
    //    { {-0.5f,  0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f } }
    //});

    emit(glm::vec3(0.5f, 0.0f, 0.0f));
    emit(glm::vec3(-0.5f, -0.5f, 0.0f));

    //std::vector<long> indices = {
    //    0, 1, 2, 2, 3, 0,
    //    4, 5, 6, 6, 7, 4,
    //};
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
    //glNamedBufferStorage(m_quadVBO, sizeof(VertexDataPosition3fColor3f) * vertices.size(), vertices.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexDataPosition3fColor3f) * m_poolSize * 4, nullptr, GL_DYNAMIC_DRAW);
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

std::array<VertexDataPosition3fColor3f, 4> ParticleGenerator::generateQuad(glm::vec3 pos, glm::vec3 color) {
    float size = 1.0f;

    std::array<VertexDataPosition3fColor3f, 4> quad;
    // left down vertice
    quad[0] = { .position = glm::vec3(pos.x, pos.y, pos.z), .color = color };
    // right down vertice
    quad[1] = { .position = glm::vec3(pos.x + size, pos.y, pos.z), .color = color };
    // right up vertice
    quad[2] = { .position = glm::vec3(pos.x + size, pos.y + size, pos.z), .color = color };
    // left up vertice
    quad[3] = { .position = glm::vec3(pos.x, pos.y + size, pos.z), .color = color };
    return quad;
}

void ParticleGenerator::render() {
    update(0.01f);
    std::vector<VertexDataPosition3fColor3f> vertices;
    for (auto& particle : m_particlePool) {
        if (!particle.active)
            continue;
        auto quad = generateQuad(particle.position, particle.color);
        vertices.insert(vertices.end(), { quad[0], quad[1], quad[2], quad[3] });
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexDataPosition3fColor3f) * vertices.size(), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(m_ShaderProgram);
    glBindVertexArray(m_quadVAO);
    glDrawElements(GL_TRIANGLES, m_nbIndexes, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
}

void ParticleGenerator::emit(glm::vec3 pos) {
    Particle& particle = m_particlePool[m_poolIndex];

    particle.position = pos;
    particle.color = glm::vec3(1.0f);
    particle.live = 100;
    particle.active = true;
    //particle.velocity = glm::vec3(-0.01f, 0.0f, 0.0f);

    m_poolIndex = --m_poolIndex % m_particlePool.size();
}

END_VISUALIZER_NAMESPACE