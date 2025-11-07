#include "camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 upV, float fovAngle, glm::vec3 target):
position(pos), up(upV), fov(fovAngle) {
    lookAt(target);
}
void Camera::lookAt(glm::vec3 target) {
    front = glm::normalize(target - position);
    yaw = glm::degrees(atan2(front.z, front.x));
    pitch = glm::degrees(asin(front.y));
    updateVectors();
}
void Camera::updateVectors() {
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(f);
}
glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}