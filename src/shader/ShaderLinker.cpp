/**
 *  Author: Arthur Lang
 *  Create Time: 2022-02-05 14:36:18
 *  Description:
 */

#include "ShaderLinker.hpp"

ShaderLinker::ShaderLinker(std::vector<Shader> shaders)
{
    _id = glCreateProgram();
    for (auto &shader : shaders) {
        if (shader.ready())
            glAttachShader(_id, shader.id());
        else
            std::cerr << "Dropping shader " << shader.id() << std::endl;
    }
    std::cout << "Linking program" << std::endl;
    glLinkProgram(_id);
    glGetProgramiv(_id, GL_LINK_STATUS, &_isLinked);
    if (_isLinked == GL_FALSE) {
        GLint maxLen = 0;
        glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &maxLen);

        std::vector<GLchar> log(maxLen);
        glGetProgramInfoLog(_id, maxLen, &maxLen, log.data());
        // cleaning ressources
        glDeleteProgram(_id);
        for (auto &shader : shaders) {
            glDeleteShader(shader.id());
        }
        std::cerr << "Shader linking failed: " << log.data() << std::endl;
    } else {
        for (auto &shader : shaders) {
            glDetachShader(_id, shader.id());
            glDeleteShader(shader.id());
        }
    }
}

ShaderLinker::~ShaderLinker()
{
}