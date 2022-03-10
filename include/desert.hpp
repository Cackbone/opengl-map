#ifndef DESERT_HPP
#define DESERT_HPP

#include <utils.hpp>
#include <GL/glew.h>

BEGIN_VISUALIZER_NAMESPACE

class Desert
{
public:
	Desert();
	~Desert();

	void load(const std::string& objFilename, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename, const std::string& textureFilename);
	void render();

private:
	int m_IndexCount;
	unsigned int m_VAO, m_VBO, m_IBO, m_ShaderProgram, m_Texture;

	void loadFromFile(const std::string& filename, std::vector<VertexDataPosition3fColor3f>& vertices, std::vector<long>& indices);
	void createVao(std::vector<VertexDataPosition3fColor3f>& vertices, std::vector<long>& indices, const std::string& textureFilename);
	void createVbo(std::vector<VertexDataPosition3fColor3f>& vertices);
	void createIbo(std::vector<long>& indices);
	void createShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	void createTexture(const std::string& filename);
	void handleShaderErrors(unsigned int shader);
};

END_VISUALIZER_NAMESPACE

#endif // !DESERT_HPP