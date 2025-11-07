#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
    public:
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 front;
    float yaw;
    float pitch;
    float fov;
    Camera(glm::vec3 pos, glm::vec3 upV, float fov, glm::vec3 target);
    void lookAt(glm::vec3 target);
    void updateVectors();
    glm::mat4 getViewMatrix();
};