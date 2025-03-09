#version 300 es

in vec4 our_Color;
out vec4 frag_Color;

void main() {
    frag_Color = our_Color;
}