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

	void load(const std::string& objFilename,
		const std::string& vertexShaderFilename,
		const std::string& fragmentShaderFilename,
		const std::string& textureFilename,
		const std::string& normalTextureFilename);
	void render();

	inline std::vector<glm::vec3>& getParticlePos() {
		return m_particlePositions;
	}

	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 textCoord;
		glm::vec3 normal;
	};

private:
	int m_IndexCount;
	unsigned int m_VAO, m_VBO, m_IBO, m_ShaderProgram, m_Texture, m_NormalTexture;
	std::vector<glm::vec3> m_particlePositions;

	void loadFromFile(const std::string& filename, std::vector<Vertex>& vertices, std::vector<long>& indices);
	void createVao(std::vector<Vertex>& vertices, std::vector<long>& indices, const std::string& textureFilename, const std::string& normalTextureFilename);
	void createVbo(std::vector<Vertex>& vertices);
	void createIbo(std::vector<long>& indices);
	void createShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	void createTexture(const std::string& filename, const std::string& normalTextureFilename);
	void handleShaderErrors(unsigned int shader);
};

END_VISUALIZER_NAMESPACE

#endif // !DESERT_HPP