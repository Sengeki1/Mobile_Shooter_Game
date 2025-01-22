#include "Camera.h"

void Camera::setCamera(int width, int height, Shader& shader, glm::mat4 (*projPtr) (int width, int height, Shader &shader)) {
    this->width = (float) width;
    this->height = (float) height;

    view_matrix = glm::lookAt(position, position + orientation, upDirection);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view_matrix));
    projection_matrix = projPtr(width, height, shader);
}

glm::mat4 Camera::getViewMatrix() {
    return view_matrix;
}

glm::mat4 Camera::getProjectionMatrix() {
    return projection_matrix;
}

void Camera::mouse(double xpos, double ypos) {
    if (firstTouch) {
        lastX = xpos;
        lastY = ypos;
        firstTouch = false;
    }

    float xoffset = float(xpos - lastX);
    float yoffset = float(lastY - ypos);
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensivity;
    yoffset *= sensivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    orientation = glm::normalize(direction);
}

