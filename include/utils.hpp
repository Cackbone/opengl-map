#ifndef UTILS_HPP
#define UTILS_HPP

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#pragma warning(pop, 0)

BEGIN_VISUALIZER_NAMESPACE

bool LoadFile(const std::string& fileName, std::string& result);
void DisplayLastWinAPIError();

struct VertexDataPosition3fColor3f
{
    glm::vec3 position;
    glm::vec3 color;
};

END_VISUALIZER_NAMESPACE

#endif // !UTILS_HPP
