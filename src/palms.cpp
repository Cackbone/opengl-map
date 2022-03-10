#include <palms.hpp>
#include <tiny_obj_loader.hpp>
#include <GL/glew.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#pragma warning(pop, 0)

BEGIN_VISUALIZER_NAMESPACE

Palms::Palms() :
    m_VAO(0), m_VBO(0), m_IBO(0), m_ShaderProgram(0)
{
}

Palms::~Palms()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteProgram(m_ShaderProgram);
}

void Palms::loadPositions(std::string filename, std::vector<glm::vec3>& positions) {
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string firstLine;
        float x, y, z, nb;
        int i = 0;

        std::getline(file, firstLine);
        positions.resize(std::stoi(firstLine));
        while (file >> x >> y >> z >> nb) {
            positions[i] = {x, y, z};
            i++;
        }
        file.close();
    }
    else {
        std::cerr << "Cannot open file: " << filename << std::endl;
    }
}

void Palms::loadFromFile(const std::string& filename, std::vector<VertexDataPosition3fColor3f>& vertices, std::vector<long>& indices)
{
    tinyobj::attrib_t attribs;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string errors;

    tinyobj::LoadObj(&attribs, &shapes, &materials, &errors, filename.c_str());

    if (errors.size() > 0) {
        std::cerr << "Error while loading obj file: " << errors << std::endl;
        return;
    }

    const size_t nb_vertices = attribs.vertices.size() / 3;

    vertices.resize(nb_vertices);
    for (unsigned long i = 0; i < nb_vertices; i++) {
        float x = attribs.vertices[3 * i];
        float y = attribs.vertices[3 * i + 1];
        float z = attribs.vertices[3 * i + 2];

        vertices[i] = {
            glm::vec3(x, y, z),
            glm::vec3(1.0f)
        };
    }

    indices.resize(shapes[0].mesh.indices.size());
    for (unsigned long i = 0; i < shapes[0].mesh.indices.size(); i++) {
        indices[i] = static_cast<long>(shapes[0].mesh.indices[i].vertex_index);
    }
}

void Palms::load(const std::string& positionsFile, const std::string& objFilename, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    std::vector<glm::vec3> positions;
    std::vector<VertexDataPosition3fColor3f> palmVertices;
    std::vector<long> palmIndices;
    std::vector<VertexDataPosition3fColor3f> vertices;
    std::vector<long> indices;

    loadPositions(positionsFile, positions);
    loadFromFile(objFilename, palmVertices, palmIndices);

    size_t palmVerticeCount = palmVertices.size();
    size_t palmIndiceCount = palmIndices.size();
    size_t positionCount = positions.size();
    vertices.resize(palmVerticeCount * positionCount);
    indices.resize(palmIndiceCount * positionCount);
    for (unsigned int positionIndex = 0; positionIndex < positionCount; positionIndex++) {
        auto& position = positions[positionIndex];

        for (unsigned long i = 0; i < palmVerticeCount; i++) {
            vertices[positionIndex * palmVerticeCount + i] = {palmVertices[i].position + position, palmVertices[i].color};
        }

        for (unsigned long i = 0; i < palmIndiceCount; i++) {
            indices[positionIndex * palmIndiceCount + i] = positionIndex * unsigned int(palmVerticeCount) + palmIndices[i];
        }
    }

    m_IndexCount = static_cast<int>(palmIndiceCount * positionCount);

    createVao(vertices, indices);
    createShaders(vertexShaderFilename, fragmentShaderFilename);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Palms::createVao(std::vector<VertexDataPosition3fColor3f>& vertices, std::vector<long>& indices)
{
    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    createVbo(vertices);
    createIbo(indices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataPosition3fColor3f), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataPosition3fColor3f), reinterpret_cast<GLvoid*>(sizeof(glm::vec3)));
}

void Palms::createVbo(std::vector<VertexDataPosition3fColor3f>& vertices)
{
    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, sizeof(VertexDataPosition3fColor3f) * vertices.size(), vertices.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void Palms::createIbo(std::vector<long>& indices)
{
    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, sizeof(long) * m_IndexCount, indices.data(), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}

void Palms::createShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
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

void Palms::handleShaderErrors(unsigned int shader)
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

void Palms::render() {
    glUseProgram(m_ShaderProgram);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
}

END_VISUALIZER_NAMESPACE