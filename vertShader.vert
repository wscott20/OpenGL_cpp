#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 iTexCoord;

out vec3 fragColor;
out vec3 fragPos;
out vec3 norm;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    vec4 worldPos = model * vec4(pos, 1.0);
    gl_Position = proj * view * worldPos;
    fragPos = worldPos.xyz;
    fragColor = color;
    texCoord = iTexCoord;
    norm = normalize(mat3(model) * normal);
}