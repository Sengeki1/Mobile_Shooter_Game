#version 100

precision mediump float;

layout (location = 0) in vec3 aPos;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(aPos, 1.0);
}