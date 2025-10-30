#pragma once
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#define pi 3.14159265358979323846f
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
inline mat4 identity() {
    return glm::mat4(1.0f);
}