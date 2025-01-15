#version 300 es

precision mediump float;

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 Normals;

uniform mat4 projection;

uniform int JOYSTICK_CONTROL;

void main() {
    vec2 pos = aPos;

    if (JOYSTICK_CONTROL == 0) { // LEFT CONTROL
        pos.x -= 8.0;
    }

    if (JOYSTICK_CONTROL == 1) { // RIGHT CONTROL
        pos.x -= 5.0;
    }

    if (JOYSTICK_CONTROL == 2) { // BOTTOM CONTROL
        pos.x -= 6.4;
        pos.y -= 1.5;
    }

    if (JOYSTICK_CONTROL == 3) { // TOP CONTROL
        pos.x -= 6.4;
        pos.y += 1.5;
    }

    gl_Position = projection * vec4(pos * 0.25, 0.0, 1.0);
}