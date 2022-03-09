#ifndef PALMS_HPP
#define PALMS_HPP

#include <utils.hpp>
#include <GL/glew.h>

BEGIN_VISUALIZER_NAMESPACE

class Palms {
public:
	Palms();
	~Palms();
	void load(const std::string& positionsFile, const std::string& objFilename, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	void render();

private:
	int m_IndexCount;
	unsigned int m_VAO, m_VBO, m_IBO, m_ShaderProgram;

	void loadFromFile(const std::string& filename, std::vector<VertexDataPosition3fColor3f>& vertices, std::vector<long>& indices);
	void createVao(std::vector<VertexDataPosition3fColor3f>& vertices, std::vector<long>& indices);
	void createVbo(std::vector<VertexDataPosition3fColor3f>& vertices);
	void createIbo(std::vector<long>& indices);
	void createShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	void handleShaderErrors(unsigned int shader);
	void loadPositions(std::string filename, std::vector<glm::vec3>& positions);
};

END_VISUALIZER_NAMESPACE

#endif // !PALMS_HPP