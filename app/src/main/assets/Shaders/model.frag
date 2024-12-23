#version 300 es

precision mediump float;

in vec3 Normal;
in vec2 texCoords;
out vec4 FragColor;
in vec3 FragPos;    // From vertex shader

uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

void main() {
    vec3 lightPos = vec3(0.0, 0.5, -3.0);
    vec3 norm = normalize(Normal);
    FragColor = vec4(vec3(shininess), 1.0);
}