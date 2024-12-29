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

vec4 transition(vec2 p) {
  float x = progress;
  x=smoothstep(.0,1.0,(x*2.0+p.x-1.0));
  return mix(getFromColor((p-.5)*(1.-x)+.5), getToColor((p-.5)*x+.5), x);
}


void main() {
    // Apply transition effect to the texture coordinates
    vec4 transitionColor = transition(TexCoord);

    // Blend the texture color with the transition effect (controlled by progress)
    FragColor = transitionColor;
}
