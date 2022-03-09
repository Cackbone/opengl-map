#include <GL/glew.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#pragma warning(pop, 0)

#include <camera.hpp>
#include <renderer.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.hpp>

#include <utils.hpp>
#include <desert.hpp>


BEGIN_VISUALIZER_NAMESPACE


bool Renderer::Initialize()
{
    m_desert.load("../../res/desert.obj", "../../res/shaders/desert.vs", "../../res/shaders/desert.fs");
    m_palms.load("../../res/palmTransfo.txt", "../../res/palm.obj", "../../res/shaders/palms.vs", "../../res/shaders/palms.fs");
    m_partGen.init();

    glCreateBuffers(1, &m_UBO);
    glNamedBufferStorage(m_UBO, sizeof(glm::mat4), glm::value_ptr(m_Camera->GetViewProjectionMatrix()), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
    m_UBOData = reinterpret_cast<glm::mat4*>(glMapNamedBufferRange(m_UBO, 0, sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT));
    return true;
}

void Renderer::Render()
{
    //m_partGen.update(0.01f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBO, 0, sizeof(glm::mat4));

    m_desert.render();
    m_palms.render();
    m_partGen.render();

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, 0, 0, 0);
}

void Renderer::Cleanup()
{
    glUnmapNamedBuffer(m_UBO);
    glDeleteBuffers(1, &m_UBO);
}

void Renderer::UpdateViewport(uint32_t width, uint32_t height)
{
    glViewport(0, 0, width, height);
    UpdateCamera();
}

void Renderer::UpdateCamera()
{
    std::memcpy(m_UBOData, glm::value_ptr(m_Camera->GetViewProjectionMatrix()), sizeof(glm::mat4));
    glFlushMappedNamedBufferRange(m_UBO, 0, sizeof(glm::mat4));
}

END_VISUALIZER_NAMESPACE
