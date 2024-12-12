#ifndef SHOOTERGAME_CUBE_H
#define SHOOTERGAME_CUBE_H

#include <GLES/egl.h> // egl servers to display what we are going to render just like glfw
#include <GLES3/gl3.h>

#include <vector>

static GLfloat vertices[] = {
        -0.5f, -0.5f,  0.5f,    //        7--------6
        0.5f, -0.5f,  0.5f,    //       /|       /|
        0.5f, -0.5f, -0.5f,    //      4--------5 |
        -0.5f, -0.5f, -0.5f,   //      | |      | |
        -0.5f,  0.5f,  0.5f,   //      | 3------|-2
        0.5f,  0.5f,  0.5f,    //      |/       |/
        0.5f,  0.5f, -0.5f,    //      0--------1
        -0.5f,  0.5f, -0.5f
};

static std::vector<int> skyboxIndices = {
        // Right
        1, 2, 6,
        6, 5, 1,
        // Left
        0, 4, 7,
        7, 3, 0,
        // Top
        4, 5, 6,
        6, 7, 4,
        // Bottom
        0, 3, 2,
        2, 1, 0,
        // Back
        0, 1, 5,
        5, 4, 0,
        // Front
        3, 7, 6,
        6, 2, 3
};

#endif //SHOOTERGAME_CUBE_H
