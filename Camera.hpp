#pragma once
#include "glm_header.hpp"
#include "OpenGL_utils.hpp"
class Camera {
    public:
    float pitch, yaw;
    vec3 position, up, front;
    mat4 view, proj; //glm::mat4
    Shader shader;
    cstr viewName, projName;
    Camera(Shader s, cstr vName, cstr pName, vec3 pos, float fov, vec3 upV, vec3 target, float aspect): shader{s}, viewName{vName}, projName{pName}, position{pos}, up{upV} {
        vec3 direction = glm::normalize(target - position);
        yaw = atan2f(direction.z, direction.x);
        float dist_xz = sqrtf(direction.x * direction.x + direction.z * direction.z);
        pitch = atan2f(direction.y, dist_xz);
        proj = glm::perspective(fov, aspect, 0.1f, 100.f);
        shader.setUniform(viewName,view);
        shader.setUniform(projName,proj);
    }
    void update() {
        vec3 f;
        f.x = cosf(pitch) * cosf(yaw);
        f.y = sinf(pitch);
        f.z = cosf(pitch) * sinf(yaw);
        front = glm::normalize(f);
        view = glm::lookAt(position, position + front, up);
        shader.setUniform(viewName,view);
    }
};