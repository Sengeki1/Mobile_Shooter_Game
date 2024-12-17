#version 300 es

precision mediump float;

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 model;
uniform float scale;

void main() {
    gl_Position = projection * model * vec4(aPos * scale, 1.0);
}