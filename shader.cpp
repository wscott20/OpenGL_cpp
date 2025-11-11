#include "shader.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
std::string read_file(std::string filename) {
    std::ifstream file(filename);
    std::string output,read;
    while (std::getline(file,read)) {
        output += read + "\n";
    }
    return output;
}
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
Shader::Shader(std::string vsFilename, std::string fsFilename) {
    std::string vsSrc = read_file(vsFilename), fsSrc = read_file(fsFilename);
    const char *vsCode = vsSrc.c_str(), *fsCode = fsSrc.c_str();   
    vert = compileShader(GL_VERTEX_SHADER, vsCode);
    frag = compileShader(GL_FRAGMENT_SHADER, fsCode);
    program = setupProgram(vert, frag);
}
void Shader::use() {
    glUseProgram(program);
}
int Shader::getLoc(const char* name) {
    int loc = glGetUniformLocation(program, name);
    if (loc == -1) std::cerr << "Error: the location of " << name << " couldn't be found" << std::endl;
    return loc;
}
void Shader::setInt(int loc, int value){
    glUniform1i(loc, value);
}
void Shader::setInt(const char* name, int value) {
    int loc = getLoc(name);
    if (loc != -1) setInt(loc, value);
}
void Shader::setFloat(int loc, float value) {
    glUniform1f(loc, value);
}
void Shader::setFloat(const char* name, float value) {
    int loc = getLoc(name);
    if (loc != -1) setFloat(loc, value);
}
void Shader::setVec2(int loc, const glm::vec2& vec) {
    glUniform2fv(loc, 1, glm::value_ptr(vec));
}
void Shader::setVec2(const char* name, const glm::vec2& vec) {
    int loc = getLoc(name);
    if (loc != -1) setVec2(loc, vec);
}
void Shader::setVec2(int loc, float x, float y) {
    glUniform2f(loc, x, y);
}
void Shader::setVec2(const char* name, float x, float y) {
    int loc = getLoc(name);
    if (loc != -1) setVec2(loc, x, y);
}
void Shader::setVec3(int loc, const glm::vec3& vec) {
    glUniform3fv(loc, 1, glm::value_ptr(vec));
}
void Shader::setVec3(const char* name, const glm::vec3& vec) {
    int loc = getLoc(name);
    if (loc != -1) setVec3(loc, vec);
}
void Shader::setVec3(int loc, float x, float y, float z) {
    glUniform3f(loc, x, y, z);
}
void Shader::setVec3(const char* name, float x, float y, float z) {
    int loc = getLoc(name);
    if (loc != -1) setVec3(loc, x, y, z);
}
void Shader::setVec4(int loc, const glm::vec4& vec) {
    glUniform4fv(loc, 1, glm::value_ptr(vec));
}
void Shader::setVec4(const char* name, const glm::vec4& vec) {
    int loc = getLoc(name);
    if (loc != -1) setVec4(loc, vec);
}
void Shader::setVec4(int loc, float x, float y, float z, float w) {
    glUniform4f(loc, x, y, z, w);
}
void Shader::setVec4(const char* name, float x, float y, float z, float w) {
    int loc = getLoc(name);
    if (loc != -1) setVec4(loc, x, y, z, w);
}
void Shader::setMat4(int loc, const glm::mat4& mat, bool transpose) {
    glUniformMatrix4fv(loc, 1, static_cast<GLboolean>(transpose), glm::value_ptr(mat));
}
void Shader::setMat4(const char* name, const glm::mat4& mat, bool transpose) {
    int loc = getLoc(name);
    if (loc != -1) setMat4(loc, mat, transpose);
}