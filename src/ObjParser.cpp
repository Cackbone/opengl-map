#include "ObjParser.hpp"

void ObjParser::parseVertex(std::vector<glm::vec3>& vertices, const std::string& line) {
    std::istringstream values(line.substr(2));
    glm::vec3 vertice;
    values >> vertice.x;
    values >> vertice.y;
    values >> vertice.z;
    vertices.push_back(vertice);
    auto toto = vertices.back();
}

bool ObjParser::parse(const std::string& file, std::vector<glm::vec3>& vertices) {
    std::ifstream stream(file, std::fstream::in);
    if (stream) {
        int i = 0;
        for (std::string line; std::getline(stream, line);) {
            auto firstChar = line[0];
            std::cout << i++ << std::endl;
            switch (firstChar) {
                case 'v':
                    parseVertex(vertices, line);
                    break;
                default:
                    break;
            }
        }
    }
    return true;
}