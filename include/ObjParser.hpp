#pragma once

#include <istream>
#pragma warning(push, 2)
#include <glm/glm.hpp>
#pragma warning(pop)

class ObjParser {
    public:
        ObjParser() = default;
        ~ObjParser() = default;

        bool parse(const std::string &file, std::vector<glm::vec3> &vertices);
        void parseVertex(std::vector<glm::vec3>& vertices, const std::string& line);
};