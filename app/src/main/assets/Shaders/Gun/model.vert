#version 300 es

precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 Normal;
out vec2 texCoords;
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 model;
uniform float scale;

void main() {
    FragPos = vec3(model * vec4(aPos * scale, 1.0));
    gl_Position = projection * model * vec4(aPos * scale, 1.0);

    Normal = aNormal;
    texCoords = aTex;
}