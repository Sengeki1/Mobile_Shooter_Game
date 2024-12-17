#ifndef SHOOTERGAME_VBO_H
#define SHOOTERGAME_VBO_H

#include <GLES/egl.h> // egl servers to display what we are going to render just like glfw
#include <GLES3/gl3.h>
#include <vector>
#include "../Libraries/glm/glm.hpp"

class VBO {
    public:
        VBO();
        void bind();
        void unbind();
        void Delete();
        void addVertices(GLsizeiptr size, GLfloat* vertices);
        void addVertices(std::vector<glm::vec3>& vertices);
        void addVertices(std::vector<glm::vec2>& vertices);
    private:
        GLuint ID;
};


#endif //SHOOTERGAME_VBO_H
