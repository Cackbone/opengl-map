#include <sun.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include <utils.hpp>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#pragma warning(pop, 0)

BEGIN_VISUALIZER_NAMESPACE



void GenerateSphereMesh(std::vector<glm::vec3>& vertices, std::vector<int>& indices, int sphereStackCount, int sphereSectorCount, glm::vec3 sphereCenter, float sphereRadius)
{
    std::size_t vertexId = 0;

    for (uint16_t i = 0; i <= sphereStackCount; ++i)
    {
        const float t = static_cast<float>(i) / static_cast<float>(sphereStackCount);

        const float stackAngle = glm::pi<float>() / 2.0f - t * glm::pi<float>();

        const float nxz = glm::cos(stackAngle);
        const float ny = glm::sin(stackAngle);

        const float xz = sphereRadius * nxz;
        const float y = sphereRadius * ny;

        const float mul = stackAngle < 0.0f ? -1.0f : 1.0f;

        const float dxz = -ny * mul;

        // When stackAngle is null dxz is equal to 0 and it is impossible to compute the tangents and bitangents
        // So we use the up vector as a bitangent
        const bool useUpVector = (sphereStackCount % 2 == 0) && (i == sphereStackCount / 2);

        for (uint16_t j = 0; j <= sphereSectorCount; ++j)
        {
            const float s = static_cast<float>(j) / static_cast<float>(sphereSectorCount);

            const float sectorAngle = s * 2.0f * glm::pi<float>();

            const float csa = glm::cos(sectorAngle);
            const float ssa = glm::sin(sectorAngle);

            const float nx = nxz * csa;
            const float nz = nxz * ssa;

            const float x = xz * csa;
            const float z = xz * ssa;

            const glm::vec3 direction = glm::vec3(x, y, z);

            const glm::vec3 position = sphereCenter + direction;

            const glm::vec2 texcoords = glm::vec2(s, t);

            const glm::vec3 normal = glm::vec3(nx, ny, nz);

            vertices[vertexId++] = position;

            if (useUpVector)
            {
                const glm::vec3 bitangent = glm::vec3(0.0f, 0.0f, 1.0f);
                const glm::vec3 tangent = glm::cross(normal, bitangent);
            }
            else
            {
                const glm::vec3 tangent = glm::normalize(glm::vec3(dxz * -glm::sin(sectorAngle), dxz * glm::cos(sectorAngle), 0.0f));
                const glm::vec3 bitangent = glm::cross(tangent, normal);
            }
        }
    }

    const int sectorCountplusOne = sphereSectorCount + 1;

    std::size_t indexID = 0;

    for (int j = 0; j < sphereSectorCount; ++j)
    {
        const int jp1 = j + 1;

        indices[indexID++] = j;
        indices[indexID++] = sectorCountplusOne + j;
        indices[indexID++] = sectorCountplusOne + jp1;
    }

    for (int i = 1; i < sphereStackCount - 1; ++i)
    {
        const int k1 = i * sectorCountplusOne;
        const int k2 = k1 + sectorCountplusOne;

        for (int j = 0; j < sphereSectorCount; ++j)
        {
            const int jp1 = j + 1;

            indices[indexID++] = k1 + j;
            indices[indexID++] = k2 + j;
            indices[indexID++] = k1 + jp1;

            indices[indexID++] = k1 + jp1;
            indices[indexID++] = k2 + j;
            indices[indexID++] = k2 + jp1;
        }
    }

    const int k1 = (sphereStackCount - 1) * sectorCountplusOne;
    const int k2 = k1 + sectorCountplusOne;

    for (int j = 0; j < sphereSectorCount; ++j)
    {
        const int jp1 = j + 1;

        indices[indexID++] = k1 + j;
        indices[indexID++] = k2 + j;
        indices[indexID++] = k1 + jp1;
    }
}

Sun::Sun() :
	m_VAO(0), m_VBO(0), m_IBO(0), m_ShaderProgram(0)
{
}

Sun::~Sun()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteProgram(m_ShaderProgram);
}

void Sun::load(const unsigned int size, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    const int sphereStackCount = 63;
    const int sphereSectorCount = 63;

    const int vertexCount = (sphereStackCount + 1) * (sphereSectorCount + 1);
    const int indexCount = (sphereStackCount - 1) * sphereSectorCount * 6;

    m_IndexCount = indexCount;

    std::vector<glm::vec3> vertices(vertexCount);
    std::vector<int> indices(indexCount);

    m_IndexCount = indexCount;

    glm::vec3 sphereCenter(1.0f, 0.0f, 0.0f);

    GenerateSphereMesh(vertices, indices, sphereStackCount, sphereSectorCount, sphereCenter, float(size));

    createVao(vertices, indices);
    createShaders(vertexShaderFilename, fragmentShaderFilename);
}

void Sun::createVao(std::vector<glm::vec3>& vertices, std::vector<int>& indices)
{
    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    createVbo(vertices);
    createIbo(indices);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
}

void Sun::createVbo(std::vector<glm::vec3>& vertices)
{
    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, sizeof(glm::vec3) * vertices.size(), vertices.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void Sun::createIbo(std::vector<int>& indices)
{
    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, sizeof(long) * m_IndexCount, indices.data(), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}

void Sun::createShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);

    m_ShaderProgram = glCreateProgram();

    glAttachShader(m_ShaderProgram, vShader);
    glAttachShader(m_ShaderProgram, fShader);

    {
        std::string vertexShader;
        LoadFile(vertexShaderFilename, vertexShader);
        const char* vertexSrc = vertexShader.c_str();
        glShaderSource(vShader, 1, &vertexSrc, nullptr);
        glCompileShader(vShader);

        handleShaderErrors(vShader);

        std::string fragmentShader;
        LoadFile(fragmentShaderFilename, fragmentShader);
        const char* fragmentSrc = fragmentShader.c_str();
        glShaderSource(fShader, 1, &fragmentSrc, nullptr);
        glCompileShader(fShader);

        handleShaderErrors(fShader);
    }

    glLinkProgram(m_ShaderProgram);

    handleShaderErrors(m_ShaderProgram);

    glDetachShader(m_ShaderProgram, vShader);
    glDetachShader(m_ShaderProgram, fShader);

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void Sun::handleShaderErrors(unsigned int shader)
{
    GLint length = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    if (length > 1)
    {
        std::string log(length, '\0');

        glGetShaderInfoLog(shader, length, nullptr, log.data());

        std::cerr << "Vertex shader log:\n" << log << '\n';
    }
}

void Sun::render()
{
    glUseProgram(m_ShaderProgram);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
}

END_VISUALIZER_NAMESPACE