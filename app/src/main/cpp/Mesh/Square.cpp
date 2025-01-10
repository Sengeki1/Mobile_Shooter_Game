#include "Square.h"

std::vector<glm::vec3> squareNormals() {
    std::vector<glm::vec3> normals;
    for (size_t i = 0; i < square_indices.size(); i += 3) {
        int index0 = square_indices[i];
        int index1 = square_indices[i + 1];
        int index2 = square_indices[i + 2];

        glm::vec2 v1 = square_vertices[index0];
        glm::vec2 v2 = square_vertices[index1];
        glm::vec2 v3 = square_vertices[index2];

        glm::vec2 E1 = v2 - v1;
        glm::vec2 E2 = v3 - v1;

        glm::vec3 normal = glm::cross(glm::vec3(E1.x, E1.y, 0.0f), glm::vec3(E2.x, E2.y, 0.0f));
        normal = glm::normalize(normal);

        normals.push_back(normal);
    }

    return normals;
}
