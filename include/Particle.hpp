#pragma once

#pragma warning(push, 0)
#include <glm/glm.hpp>
#pragma warning(pop, 0)

struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 colorStart = glm::vec3(1.0f);
        glm::vec3 colorEnd = glm::vec3(0.0f);
        float size = 1.0f;
        float live; // in seconds
        bool active = false;
};