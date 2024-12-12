#ifndef SHOOTERGAME_VAO_H
#define SHOOTERGAME_VAO_H

#include <GLES/egl.h> // egl servers to display what we are going to render just like glfw
#include <GLES3/gl3.h>

class VAO {
    public:
        VAO();
        void bind();
        void unbind();
        void Delete();
        void LinkAttrib(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    private:
        GLuint ID;
};


#endif //SHOOTERGAME_VAO_H
