#include <GL/glew.h>

BEGIN_VISUALIZER_NAMESPACE

void CheckGLError(const std::string_view& functionName, const std::string_view& fileName, const int32_t lineNumber)
{
    GLenum err = glGetError();

    while (err != GL_NO_ERROR)
    {
        std::string_view error = "Unknown error";

        switch (err)
        {
        case GL_INVALID_ENUM:
            error = "invalid enum";
            break;
        case GL_INVALID_VALUE:
            error = "invalid value";
            break;
        case GL_INVALID_OPERATION:
            error = "invalid operation";
            break;
        case GL_OUT_OF_MEMORY:
            error = "out of memory";
            break;
        case GL_STACK_UNDERFLOW:
            error = "stack underflow";
            break;
        case GL_STACK_OVERFLOW:
            error = "stack overflow";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "invalid framebuffer operation";
            break;
        }

        std::cerr << "Error in file " << fileName << " line " << lineNumber << " when calling " << functionName << ": " << error << '\n';
        err = glGetError();
    }
}

bool CompileShader(const std::string_view& str, uint32_t& shader, uint32_t shaderType)
{
    shader = glCreateShader(shaderType);

    char const *const shaderSource = str.data();

    glShaderSource(shader, 1, &shaderSource, nullptr);

    glCompileShader(shader);

    {
        GLint length = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        if (length > 1)
        {
            std::string log(length, '\0');

            glGetShaderInfoLog(shader, length, nullptr, log.data());

            if (shaderType == GL_VERTEX_SHADER)
            {
                std::cerr << "Vertex shader log:\n" << log << '\n';
            }
            else if (shaderType == GL_FRAGMENT_SHADER)
            {
                std::cerr << "Fragment shader log:\n" << log << '\n';
            }
            else if (shaderType == GL_COMPUTE_SHADER)
            {
                std::cerr << "Compute shader log:\n" << log << '\n';
            }
            else
            {
                std::cerr << "Shader log:\n" << log << '\n';
            }
        }
    }

    GLint isCompiled = 0;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    return isCompiled;
}

END_VISUALIZER_NAMESPACE
