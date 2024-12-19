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

void VBO::addVertices(std::vector<glm::vec3>& vertices) {
    glBufferData(GL_ARRAY_BUFFER, vertices.size()  * sizeof(GLfloat) * 3, vertices.data(), GL_STATIC_DRAW);
}

void VBO::addVertices(std::vector<glm::vec2>& vertices) {
    glBufferData(GL_ARRAY_BUFFER, vertices.size()  * sizeof(GLfloat) * 2, vertices.data(), GL_STATIC_DRAW);
}


void VBO::Delete(){
    glDeleteBuffers(1, &ID);
};

