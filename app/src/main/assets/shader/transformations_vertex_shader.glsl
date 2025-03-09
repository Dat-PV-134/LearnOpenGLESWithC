#version 300 es

layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec4 a_Color;

out vec4 our_Color;

void main() {
    gl_Position = vec4(a_Pos.x, a_Pos.y, 1.0, 1.0);
    our_Color = a_Color;
}