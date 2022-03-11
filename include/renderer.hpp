#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <chrono>

#include "shader/ShaderLinker.hpp"
#include "shader/Shader.hpp"
#include "utils.hpp"
#include "ParticleGenerator.hpp"

#include <desert.hpp>
#include <palms.hpp>

BEGIN_VISUALIZER_NAMESPACE

class Camera;

class Renderer
{
public:
    Renderer(const std::shared_ptr<Camera>& camera)
        : m_Camera(camera)
    {}

    Renderer() = delete;
    ~Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    bool Initialize();
    void Render();
    void Cleanup();

    void UpdateViewport(uint32_t width, uint32_t height);
    void UpdateCamera();

private:
    int m_IndexCount;
    GLuint m_UBO;

    glm::mat4* m_UBOData = nullptr;

    Desert m_desert;
    Palms m_palms;

    std::shared_ptr<Camera> m_Camera;
    ParticleGenerator m_partGen;

    std::chrono::system_clock::time_point m_lastRenderCall;
};

END_VISUALIZER_NAMESPACE

#endif // !RENDERER_HPP
