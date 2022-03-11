#ifndef RENDERER_HPP
#define RENDERER_HPP


#include <desert.hpp>
#include <palms.hpp>
#include <sun.hpp>
#include <skybox.hpp>

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
    void UpdateUniforms();
    void updateLightPos();

    struct RendererUniforms {
        glm::mat4 viewProjectionMatrix;
        glm::vec3 lightPos;
    };

private:
    int m_IndexCount;
    GLuint m_UBO;

    RendererUniforms* m_UBOData = nullptr;

    Desert m_desert;
    Palms m_palms;
    Sun m_sun;
    Skybox m_skybox;

    std::shared_ptr<Camera> m_Camera;
    glm::vec3 m_lightPos;
    float m_lightAngle;
    float m_lightMovementRadius;
};

END_VISUALIZER_NAMESPACE

#endif // !RENDERER_HPP
