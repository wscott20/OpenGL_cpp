#version 330 core
in vec2 tc;
out vec4 color;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;

void main() {
    float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, tc).rgb;
    color = vec4(pow(
        hdr ? vec3(1.0) - exp(-hdrColor * exposure) : hdrColor,
        vec3(1.0) / gamma
    ), 1.0);
}