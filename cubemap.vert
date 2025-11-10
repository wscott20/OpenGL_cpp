#version 330 core
layout(location = 0) in vec3 pos;
out vec3 texDir;

uniform mat4 proj;
uniform mat4 view;
void main() {
    texDir = pos;
    gl_Position = proj * view * vec4(pos, 1);
}