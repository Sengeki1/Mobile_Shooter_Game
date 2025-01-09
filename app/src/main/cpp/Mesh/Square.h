#ifndef SHOOTERGAME_SQUARE_H
#define SHOOTERGAME_SQUARE_H

#include <iostream>
#include <GLES3/gl3.h>
#include <vector>

static GLfloat square_vertices[] = {    -0.5,  0.5,
                          -0.5, -0.5,
                           0.5, -0.5,
                           0.5,  0.5      };

static std::vector<int>square_indices = {     0, 1, 3, 3, 1, 2    };

#endif //SHOOTERGAME_SQUARE_H
