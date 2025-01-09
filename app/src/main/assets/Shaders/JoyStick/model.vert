#version 300 es

precision mediump float;

layout (location = 0) in vec2 aPos;

uniform mat4 projection;

void main() {
    vec2 pos = aPos;
    pos.x -= 5.7;
    gl_Position = projection * vec4(pos * 0.3, 0.0, 1.0);
}