#version 300 es

precision mediump float;

in vec3 Normal;
in vec2 texCoords;
out vec4 FragColor;

uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

void main() {
    vec3 norm = ((normalize(Normal) * vec3(0.0, 0.0, 1.0)) + vec3(0.8, 0.7, 0.2)) * vec3(0.0, 0.0, 1.0);
    norm += vec3(0.7, 0.4, 0.0);  // Add a base color

    if (norm.b > 0.5) {
        norm = vec3(0.87, 0.68, 0.48);
    } else {
        norm = vec3(0.9, 0.7, 0.5); // skin color (example)
    }

    // Final color output
    FragColor = vec4(norm, 1.0);
}