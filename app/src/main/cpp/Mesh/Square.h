#ifndef SHOOTERGAME_SQUARE_H
#define SHOOTERGAME_SQUARE_H

#include <iostream>
#include <GLES3/gl3.h>
#include "../Libraries/glm/glm.hpp"
#include <vector>

static std::vector<glm::vec2> square_vertices = {
        glm::vec2(-0.5f, -0.5f), // bottom left
        glm::vec2(0.5f, -0.5f),  // bottom right
        glm::vec2(0.5f,  0.5f),  // top right
        glm::vec2(-0.5f, 0.5f)   // top left
};

static std::vector<unsigned int>square_indices = {     0, 1, 2, 2, 3, 0    };

std::vector<glm::vec3> squareNormals();

#endif //SHOOTERGAME_SQUARE_H
