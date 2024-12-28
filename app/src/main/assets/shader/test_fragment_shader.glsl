#version 300 es

in vec3 ourColor;          // Color passed from the vertex shader (not used here)
in vec2 TexCoord;         // Texture coordinates passed from the vertex shader

out vec4 FragColor;       // Final output color of the fragment

uniform sampler2D texture1;   // The texture to transition to
uniform sampler2D texture2;   // The texture to transition from
uniform vec2 direction; // Transition direction (e.g., left to right)
uniform float progress;       // Transition progress (from 0.0 to 1.0)

vec4 getToColor(vec2 uv) {
    return texture(texture1, uv); // Color from the texture to transition *to*
}

vec4 getFromColor(vec2 uv) {
    return texture(texture2, uv); // Color from the texture to transition *from*
}

vec4 transition(vec2 uv) {
    vec2 hello = vec2(1.0, 0.0);
    vec2 p = uv + progress * hello; // Apply progress to the direction
    vec2 f = fract(p);                  // Get fractional part of coordinates
    return mix(
        getToColor(f),                  // Color from the "to" texture
        getFromColor(f),                // Color from the "from" texture
        step(0.0, p.y) * step(p.y, 1.0) * step(0.0, p.x) * step(p.x, 1.0)
    );
}

void main() {
    // Apply transition effect to the texture coordinates
    vec4 transitionColor = transition(TexCoord);

    // Blend the texture color with the transition effect (controlled by progress)
    FragColor = transitionColor;
}
