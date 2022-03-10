#include <desert.hpp>
#include <tiny_obj_loader.hpp>
#include <stb_image.h>
#include <GL/glew.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#pragma warning(pop, 0)

BEGIN_VISUALIZER_NAMESPACE

Desert::Desert() :
    m_IBO(0), m_VAO(0), m_VBO(0), m_ShaderProgram(0)
{

}

void Desert::load(const std::string& objFilename, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename, const std::string& textureFilename)
{
    std::vector<VertexDataPosition3fColor3f> vertices;
    std::vector<long> indices;
    loadFromFile(objFilename, vertices, indices);

    createVao(vertices, indices, textureFilename);
    createShaders(vertexShaderFilename, fragmentShaderFilename);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

Desert::~Desert()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteProgram(m_ShaderProgram);
}

void Desert::createVao(std::vector<VertexDataPosition3fColor3f>& vertices, std::vector<long>& indices, const std::string &textureFilename)
{
    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    createVbo(vertices);
    createIbo(indices);
    createTexture(textureFilename);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataPosition3fColor3f), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataPosition3fColor3f), reinterpret_cast<GLvoid*>(sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataPosition3fColor3f), reinterpret_cast<GLvoid*>(sizeof(glm::vec3) * 2));
}

void Desert::createVbo(std::vector<VertexDataPosition3fColor3f>& vertices)
{
    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, sizeof(VertexDataPosition3fColor3f) * vertices.size(), vertices.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void Desert::createIbo(std::vector<long>& indices)
{
    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, sizeof(long) * m_IndexCount, indices.data(), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}

void Desert::createShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
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

void Desert::handleShaderErrors(unsigned int shader)
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

void Desert::loadFromFile(const std::string& filename, std::vector<VertexDataPosition3fColor3f>& vertices, std::vector<long>& indices)
{
    tinyobj::attrib_t attribs;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string errors;

    tinyobj::LoadObj(&attribs, &shapes, &materials, &errors, filename.c_str());

    const size_t nb_vertices = attribs.vertices.size() / 3;
    float min[3] = { -255.875f, -44.3907776f, -255.875f };
    float max[3] = { 255.875f, -12.0602303f, 255.875f };

    vertices.resize(nb_vertices);
    for (unsigned long i = 0; i < nb_vertices; i++) {
        float x = attribs.vertices[3 * i];
        float y = attribs.vertices[3 * i + 1];
        float z = attribs.vertices[3 * i + 2];
        float color = (y - min[1]) / (max[1] - min[1]) - 0.6f;

        vertices[i] = {
            glm::vec3(x, y, z),
            glm::vec3({ 0.9294f + color, 0.7882f + color , 0.686f + color}),
            glm::vec2(0)
        };
    }

    const size_t texCoordsCount = attribs.texcoords.size();
    indices.resize(shapes[0].mesh.indices.size());
    for (unsigned long i = 0; i < shapes[0].mesh.indices.size(); i++) {
        const unsigned int vertex_index = shapes[0].mesh.indices[i].vertex_index;
        const unsigned int texcoord_index = shapes[0].mesh.indices[i].texcoord_index;
        indices[i] = static_cast<long>(vertex_index);
        if (texCoordsCount > 0) {
            vertices[vertex_index].textCoord.x = attribs.texcoords[2 * texcoord_index];
            vertices[vertex_index].textCoord.y = attribs.texcoords[2 * texcoord_index + 1];
        }
    }

    m_IndexCount = static_cast<int>(indices.size());
}

void Desert::createTexture(const std::string& filename)
{
    int width, height, channels;

    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture file" << std::endl;
    }
    stbi_image_free(data);
}

void Desert::render() {
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glUseProgram(m_ShaderProgram);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
}

END_VISUALIZER_NAMESPACE
