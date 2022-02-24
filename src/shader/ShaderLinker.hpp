/**
 *  Author: Arthur Lang
 *  Create Time: 2022-02-05 14:36:23
 *  Description:
 */

#pragma once

#include "Shader.hpp"

class ShaderLinker
{
    public:
        ShaderLinker(const std::vector<Shader> shaders);
        ~ShaderLinker();

        inline GLuint id() {
            return _id;
        }

        inline bool ready() {
            return _isLinked != GL_FALSE;
        }

    private:
        GLuint _id;
        GLint _isLinked = GL_FALSE;
};