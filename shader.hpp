#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
using uint = unsigned int;
struct Shader {
    uint vert, frag, program;
    Shader(const char* vsCode, const char* fsCode);
    void use();
    int getLoc(const char* name);
    void setInt(int loc, int value);
    void setInt(const char* name, int value);
    void setFloat(int loc, float value);
    void setFloat(const char* name, float value);
    void setVec2(int loc, const glm::vec2& vec);
    void setVec2(const char* name, const glm::vec2& vec);
    void setVec2(int loc, float x, float y);
    void setVec2(const char* name, float x, float y);
    void setVec3(int loc, const glm::vec3& vec);
    void setVec3(const char* name, const glm::vec3& vec);
    void setVec3(int loc, float x, float y, float z);
    void setVec3(const char* name, float x, float y, float z);
    void setVec4(int loc, const glm::vec4& vec);
    void setVec4(const char* name, const glm::vec4& vec);
    void setVec4(int loc, float x, float y, float z, float w);
    void setVec4(const char* name, float x, float y, float z, float w);
    void setMat4(int loc, const glm::mat4& mat, bool transpose = false);
    void setMat4(const char* name, const glm::mat4& mat, bool transpose = false);
};