#version 300 es
in vec3 ourColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(ourColor.x, ourColor.y, ourColor.z, 1.0f);
}