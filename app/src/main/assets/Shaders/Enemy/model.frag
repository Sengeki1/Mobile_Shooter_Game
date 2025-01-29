#version 300 es

precision mediump float;

in vec3 Normal;
in vec2 texCoords;
out vec4 FragColor;
in vec3 FragPos;    // From vertex shader

uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
uniform vec3 cameraPos;

void main() {
    float specularStrength = 0.1;
    vec3 lightPos = vec3(0.0, 0.9, -9.0);
    vec3 lightDir = normalize(lightPos - FragPos);

    // diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);

    // specular
    vec3 cameraDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // invert the lightDir
    float spec = pow(max(dot(reflectDir, cameraDir), 0.0f), shininess);

    vec3 result = (diff * vec3(1.0f, 0.0f, 0.0f)) + (specularStrength * spec);

    FragColor = vec4(result, 1.0);
}