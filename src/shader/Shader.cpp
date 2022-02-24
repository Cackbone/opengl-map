/**
 *  Author: Arthur Lang
 *  Create Time: 2022-02-05 12:00:00
 *  Description:
 */

#include "Shader.hpp"

Shader::Shader(int shaderType, const char *shaderPath)
{
    _id = glCreateShader(shaderType);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(shaderPath, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();

        std::cout << "Compiling shader: " << shaderPath << std::endl;
    	char const * VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(_id, 1, &VertexSourcePointer, 0);
        glCompileShader(_id);

        // check compilation
        glGetShaderiv(_id, GL_COMPILE_STATUS, &_isCompile);
        if (_isCompile == GL_FALSE) {
            GLint maxLen = 0;
            glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &maxLen);

            std::vector<GLchar> log(maxLen);
            glGetShaderInfoLog(_id, maxLen, &maxLen, log.data());

            glDeleteShader(_id);

            std::cerr << "Shader compilation failed: \n" << log.data() << std::endl;
        }
	} else
        std::cerr << "Error opening shader file" << std::endl;
}

Shader::~Shader()
{
}