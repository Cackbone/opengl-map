#pragma once

#include <istream>
#include <glm/glm.hpp>

class ObjParser {
    public:
        ObjParser() = default;
        ~ObjParser() = default;

        bool parse(const std::string &file, std::vector<glm::vec3> &vertices);
};