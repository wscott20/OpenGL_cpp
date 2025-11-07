#version 330 core
in vec3 fragPos;
in vec3 fragColor;
in vec3 fragNorm;
in vec2 fragTC;

//uniform sampler2D tex;
//uniform vec3 lightPos;
out vec4 color;

void main() {
    color = vec4(1,1,1,1);
}