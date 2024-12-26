#version 300 es

precision mediump float;

out vec4 FragColor;
in vec3 texCoords;

uniform samplerCube skybox;

void main() {
    FragColor = texture(skybox, texCoords);
}