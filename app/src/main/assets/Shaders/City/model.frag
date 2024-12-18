#version 300 es

precision mediump float;

in vec3 Normal;
in vec2 texCoords;
out vec4 FragColor;

uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

void main() {
    vec3 norm = normalize(Normal);
    FragColor = vec4(1.0);
}