#ifndef SHOOTERGAME_VBO_H
#define SHOOTERGAME_VBO_H

#include <GLES/egl.h> // egl servers to display what we are going to render just like glfw
#include <GLES3/gl3.h>

class VBO {
    public:
        VBO();
        void bind();
        void unbind();
        void Delete();
        void addVertices(GLsizeiptr size, GLfloat* vertices);
    private:
        GLuint ID;
};


#endif //SHOOTERGAME_VBO_H
