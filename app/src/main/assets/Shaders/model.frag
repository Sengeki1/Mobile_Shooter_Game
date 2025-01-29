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

uniform int ID;

void main() {
    float specularStrength = 0.1;
    vec3 lightPos;
    vec3 ambient = 0.1 * vec3(0.7f);
    if (ID == 0) {
        lightPos = vec3(0.0, 15.0, 0.0);
    } else {
        lightPos = vec3(0.0, 0.9, -9.0);
    }

    vec3 lightDir = normalize(lightPos - FragPos);

    // diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);

    // specular
    vec3 cameraDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // invert the lightDir
    float spec = pow(max(dot(reflectDir, cameraDir), 0.0f), shininess);

    vec3 result;
    if (ID == 0) {
        result = (diff * diffuse) + ambient;
    } else {
        result = (diff * diffuse) + (specularStrength * spec * specular);
    }

    FragColor = vec4(result, 1.0);
}