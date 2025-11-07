#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 fragPos;
out vec3 fragNorm;
out vec2 fragTC;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    vec4 worldPos = model * vec4(pos, 1.0);
    gl_Position = proj * view * worldPos;
    fragPos = worldPos.xyz;
    fragNorm = mat3(transpose(inverse(model))) * normal;
    fragTC = texCoord;
}