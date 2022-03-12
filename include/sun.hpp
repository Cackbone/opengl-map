#ifndef SUN_HPP
#define SUN_HPP

#include <utils.hpp>
#include <GL/glew.h>

BEGIN_VISUALIZER_NAMESPACE

class Sun
{
public:
	Sun();
	~Sun();
	void load(const unsigned int size, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	void render();

private:
	int m_IndexCount;
	unsigned int m_VAO, m_VBO, m_IBO, m_ShaderProgram;

	void createVao(std::vector<glm::vec3> &vertices, std::vector<int> &indices);
	void createVbo(std::vector<glm::vec3>& vertices);
	void createIbo(std::vector<int>& indices);
	void createShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	void handleShaderErrors(unsigned int shader);
};

END_VISUALIZER_NAMESPACE

#endif // !SUN_HPP