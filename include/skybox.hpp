#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <GL/glew.h>

BEGIN_VISUALIZER_NAMESPACE

class Skybox
{
public:
	Skybox();
	~Skybox();
	void load(const std::vector<std::string>& filenames, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	void render();

private:
	int m_IndexCount;
	unsigned int m_VAO, m_VBO, m_IBO, m_ShaderProgram, m_Texture;

	void loadTextures(const std::vector<std::string>& filenames);
	void createShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	void handleShaderErrors(unsigned int shader);
};

END_VISUALIZER_NAMESPACE

#endif // !SKYBOX_HPP