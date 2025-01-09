#ifndef SHOOTERGAME_SQUARE_H
#define SHOOTERGAME_SQUARE_H

#include <iostream>
#include <GLES3/gl3.h>
#include <vector>

static GLfloat square_vertices[] = {
        -0.5f, -0.5f,  // bottom left
        0.5f, -0.5f,  // bottom right
        0.5f,  0.5f,  // top right
        -0.5f,  0.5f   // top left
};

static std::vector<unsigned int>square_indices = {     0, 1, 2, 2, 3, 0    };

#endif //SHOOTERGAME_SQUARE_H
