#include "EBO.h"

EBO::EBO(std::vector<int> indices) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); // Bind the buffer VBO with Vertex attributes
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
}

void EBO::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
    glDeleteBuffers(1, &ID);
}
