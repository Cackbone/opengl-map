#include "ObjParser.hpp"

bool ObjParser::parse(const std::string& file, std::vector<glm::vec3>& vertices) {
    std::ifstream stream(file, std::fstream::in);
    if (stream) {
        for (std::string line; std::getline(stream, line);) {

        }
    }
    return false;
}