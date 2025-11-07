#include "shader.hpp"
#include <iostream>
uint compileShader(GLenum type, const char* source) {
    uint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << (
            type == GL_VERTEX_SHADER ? "Vertex" :
            type == GL_FRAGMENT_SHADER ? "Fragment" :
            "Geometry"
        ) << " Shader Compilation Failed\n" << infoLog << std::endl;
    }
    return shader;
}
uint setupProgram(uint vs, uint fs) {
    uint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader Program Linking Failed\n" << infoLog << std::endl;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}