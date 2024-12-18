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
    vec3 norm = (normalize(Normal) * vec3(0.0, 0.0, 1.0));

    if (norm.b > 0.5) {
        norm = vec3(0.05);
    } else {
        norm = vec3(0.01);
    }
    FragColor = vec4(normalize(Normal), 1.0);
}