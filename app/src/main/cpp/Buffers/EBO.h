#ifndef SHOOTERGAME_EBO_H
#define SHOOTERGAME_EBO_H

#include <GLES/egl.h> // egl servers to display what we are going to render just like glfw
#include <GLES3/gl3.h>

#include <vector>

class EBO {
    public:
        GLuint ID;
        EBO();
        EBO(std::vector<int> indices);
        EBO(std::vector<unsigned int> indices);

        void bind();
        void addIndices(std::vector<unsigned int> indices);
        void unbind();
        void Delete();
};


#endif //SHOOTERGAME_EBO_H
