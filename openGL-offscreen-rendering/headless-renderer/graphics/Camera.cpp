#include "Camera.h"
#include "constants/Maps.h"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::getProjectionMatrix(int width, int height) {
    return glm::perspective(glm::radians((float)fov), (float)(width) / float(height), zNear, zFar);
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(_position, _position + _cameraFront, Constants::axisVectors.at(Constants::VectorType::Y));
}