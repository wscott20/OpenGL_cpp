#pragma once
#include <glad/glad.h>
using uint = unsigned int;
uint compileShader(GLenum type, const char* source);
uint setupProgram(uint vs, uint fs);