#pragma once

#pragma warning(push, 0)
#include <glm/glm.hpp>
#pragma warning(pop, 0)

struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 color = glm::vec3(0.5f);
        float live;
        bool active = false;
};