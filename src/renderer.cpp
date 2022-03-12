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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <utils.hpp>
#include <desert.hpp>


BEGIN_VISUALIZER_NAMESPACE


bool Renderer::Initialize()
{
    std::vector<std::string> skyboxTextures = {
        "../../res/skybox/right.png",
        "../../res/skybox/left.png",
        "../../res/skybox/top.png",
        "../../res/skybox/bottom.png",
        "../../res/skybox/front.png",
        "../../res/skybox/back.png"
    };

    m_desert.load("../../res/objs/desert_texture.obj", "../../res/shaders/desert.vs", "../../res/shaders/desert.fs", "../../res/sand.png", "../../res/sand_normal.png");
    m_palms.load("../../res/palmTransfo.txt", "../../res/objs/palm_color.obj", "../../res/shaders/palms.vs", "../../res/shaders/palms.fs");
    m_sun.load(20, "../../res/shaders/sun.vs", "../../res/shaders/sun.fs");
    m_skybox.load(skyboxTextures, "../../res/shaders/skybox.vs", "../../res/shaders/skybox.fs");

    m_lightPos = { 0.0f, 100.0f, 0.0f };
    m_lightMovementRadius = 300.0f;

    RendererUniforms uniforms = {
        m_Camera->GetViewProjectionMatrix(),
        m_lightPos
    };

    glCreateBuffers(1, &m_UBO);
    glNamedBufferStorage(m_UBO, sizeof(RendererUniforms), &uniforms, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
    m_UBOData = reinterpret_cast<RendererUniforms*>(glMapNamedBufferRange(m_UBO, 0, sizeof(RendererUniforms), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT));
    return true;
}

void Renderer::Render()
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBO, 0, sizeof(RendererUniforms));

    m_desert.render();
    m_palms.render();
    m_sun.render();
    m_skybox.render();

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
    UpdateUniforms();
}

void Renderer::updateLightPos()
{
    m_lightAngle += 0.008f;
    m_lightPos.x = m_lightMovementRadius * cos(m_lightAngle);
    m_lightPos.z = m_lightMovementRadius * sin(m_lightAngle);
    m_lightPos.y = std::max(m_lightMovementRadius * sin(m_lightAngle), m_lightMovementRadius * -0.25f);
    UpdateUniforms();
}

void Renderer::UpdateUniforms()
{
    RendererUniforms uniforms = {
        m_Camera->GetViewProjectionMatrix(),
        m_lightPos
    };
    std::memcpy(m_UBOData, &uniforms, sizeof(RendererUniforms));
    glFlushMappedNamedBufferRange(m_UBO, 0, sizeof(RendererUniforms));
}

END_VISUALIZER_NAMESPACE
