#ifndef GLUTILS_HPP
#define GLUTILS_HPP

BEGIN_VISUALIZER_NAMESPACE

void CheckGLError(const std::string_view& functionName, const std::string_view& fileName, const int32_t lineNumber);
bool CompileShader(const std::string_view& str, uint32_t& shader, uint32_t shaderType);

template<std::size_t shaderCount>
bool LinkShaderProgram(const std::array<uint32_t, shaderCount>& shaders, uint32_t& shaderProgram)
{
    shaderProgram = glCreateProgram();

    for (const uint32_t& shader : shaders)
    {
        glAttachShader(shaderProgram, shader);
    }

    glLinkProgram(shaderProgram);

    for (const uint32_t& shader : shaders)
    {
        glDetachShader(shaderProgram, shader);
        glDeleteShader(shader);
    }

    {
        GLint length = 0;

        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);

        if (length > 1)
        {
            std::string log(length, '\0');

            glGetProgramInfoLog(shaderProgram, length, nullptr, log.data());

            std::cerr << "Shader program log:\n" << log << '\n';
        }
    }

    GLint isLinked = 0;

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);

    return isLinked;
}

// Define this to 0 if you don't want to check errors anymore
#define DEBUG_GL_CALLS 1

#if DEBUG_GL_CALLS
#define GL_CALL(x, ...)    x(__VA_ARGS__); CheckGLError(#x, __FILE__, __LINE__)
#define GL_CALL_REINTERPRET_CAST_RETURN_VALUE(type, x, ...)    reinterpret_cast<type>(x(__VA_ARGS__)); CheckGLError(#x, __FILE__, __LINE__)
#else
#define GL_CALL(x, ...)    x(__VA_ARGS__);
#define GL_CALL_REINTERPRET_CAST_RETURN_VALUE(type, x, ...)    reinterpret_cast<type>(x(__VA_ARGS__));
#endif

END_VISUALIZER_NAMESPACE

#endif // !GLUTILS_HPP
