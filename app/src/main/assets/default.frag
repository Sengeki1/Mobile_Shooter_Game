#version 300 es

precision mediump float;

uniform vec2 uv_resolution;
uniform float u_time;
out vec4 frag_color;

void main() {
    vec2 uv = (gl_FragCoord.xy / uv_resolution) * 2.;
    uv.x *= uv_resolution.x / uv_resolution.y;

    frag_color = vec4(uv, 0.0, 1.0);
}