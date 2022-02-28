/**
 *  Author: Arthur Lang
 *  Create Time: 2022-02-05 12:00:05
 *  Description:
 */

#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

class Shader {

    public:
        Shader(int shaderType, const char *shaderPath);
        ~Shader();

        inline GLuint id() {
            return _id;
        }
        inline bool ready() {
            return _isCompile != GL_FALSE;
        }

    private:
        GLuint _id;
        GLint _isCompile = 0;
};