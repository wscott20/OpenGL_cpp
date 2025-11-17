#version 330 core
in vec2 tc;
out vec4 color;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;
uniform bool gammaCorrection;

void main() {
    float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, tc).rgb;
    vec3 result = hdr ? vec3(1.0) - exp(-hdrColor * exposure) : hdrColor;
    if (gammaCorrection) result = pow(result, vec3(1.0) / gamma);
    color = vec4(result, 1.0);
}