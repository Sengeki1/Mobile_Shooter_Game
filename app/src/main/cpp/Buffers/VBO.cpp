#include "VBO.h"

VBO::VBO(){
    glGenBuffers(1, &ID);
};

void VBO::bind(){
    glBindBuffer(GL_ARRAY_BUFFER, ID);
};

void VBO::unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
};

void VBO::addVertices(GLsizeiptr size, GLfloat* vertices){
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
};

void VBO::Delete(){
    glDeleteBuffers(1, &ID);
};

