#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texcoords;

out vec2 tc;

void main() {
    tc = texcoords;
    gl_Position = vec4(pos,1);
}