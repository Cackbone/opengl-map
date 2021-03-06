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

void Palms::loadFromFile(const std::string& filename, std::vector<Vertex>& vertices, std::vector<long>& indices)
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
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec2(0),
            glm::vec3(0)
        };
    }

    const size_t texCoordsCount = attribs.texcoords.size();
    const size_t normalCount = attribs.normals.size();
    indices.resize(shapes[0].mesh.indices.size() + shapes[1].mesh.indices.size());
    unsigned long indices_index = 0;
    for (unsigned int shape = 0; shape < 2; shape++) {
        for (unsigned long i = indices_index; i < shapes[shape].mesh.indices.size(); i++) {
            const unsigned int vertex_index = shapes[shape].mesh.indices[i].vertex_index;
            indices[i] = static_cast<long>(vertex_index);

            if (texCoordsCount > 0) {
                const unsigned int texcoord_index = shapes[shape].mesh.indices[i].texcoord_index;
                vertices[vertex_index].textCoord.x = attribs.texcoords[2 * texcoord_index];
                vertices[vertex_index].textCoord.y = attribs.texcoords[2 * texcoord_index + 1];
            }
            if (normalCount > 0) {
                const unsigned int normal_index = shapes[shape].mesh.indices[i].normal_index;
                vertices[vertex_index].normal.x = attribs.normals[3 * normal_index];
                vertices[vertex_index].normal.y = attribs.normals[3 * normal_index + 1];
                vertices[vertex_index].normal.z = attribs.normals[3 * normal_index + 2];
            }

            if (shape == 0) { // Wood
                vertices[vertex_index].color = glm::vec3(0.491, 0.262, 0.125);
            }
            else {
                vertices[vertex_index].color = glm::vec3(0.034, 0.451, 0.034);
            }
        }
        indices_index += unsigned long(shapes[shape].mesh.indices.size());
    }
}

void Palms::load(const std::string& positionsFile, const std::string& objFilename, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    std::vector<glm::vec3> positions;
    std::vector<Vertex> palmVertices;
    std::vector<long> palmIndices;
    std::vector<Vertex> vertices;
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
            vertices[positionIndex * palmVerticeCount + i] = {
                palmVertices[i].position + position, // position
                palmVertices[i].color,               // color
                palmVertices[i].textCoord,           // texCoords
                palmVertices[i].normal               // normal
            };
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

void Palms::createVao(std::vector<Vertex>& vertices, std::vector<long>& indices)
{
    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    createVbo(vertices);
    createIbo(indices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(sizeof(glm::vec3) * 2));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(sizeof(glm::vec3) * 2 + sizeof(glm::vec2)));
}

void Palms::createVbo(std::vector<Vertex>& vertices)
{
    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, sizeof(Vertex) * vertices.size(), vertices.data(), 0);
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