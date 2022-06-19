#include "Camera.hpp"
#include "../constants/Maps.hpp"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::getProjectionMatrix(int width, int height) {
    return glm::perspective(_fov, (float)(width) / float(height), _zNear, _zFar);
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(glm::vec3(_position), glm::vec3(_target), Constants::Y_AXIS_3);
}

void Camera::moveCameraForeward()
{
    glm::vec4 direction = _target - _position;
	_position += direction * _zoomSpeed;
}

void Camera::moveCameraBack()
{
    glm::vec4 direction = _target - _position;
	_position -= direction * _zoomSpeed;
}

void Camera::moveCameraLeft()
{ 
	glm::vec3 direction = _target - _position;
	glm::vec3 slide_vector = glm::normalize(glm::cross(direction, Constants::VEC_UP_3));
	glm::vec3 rightDirection = slide_vector * _translationSpeed;
	_position += glm::vec4(rightDirection, 0.0);
	_target += glm::vec4(rightDirection, 0.0);}

void Camera::moveCameraRight()
{
    glm::vec3 direction = _target - _position;
	glm::vec3 slide_vector = glm::normalize(glm::cross(direction, Constants::VEC_UP_3));
	glm::vec3 rightDirection = slide_vector * _translationSpeed;
	_position -= glm::vec4(rightDirection, 0.0);
	_target -= glm::vec4(rightDirection, 0.0);
}

void Camera::moveCameraUp()
{
	glm::vec3 direction = _target - _position;
	glm::vec3 slide_vector = glm::normalize(glm::cross(direction, Constants::VEC_UP_3));
	glm::vec3 upDirection = glm::cross(direction, slide_vector) * _translationSpeed;
	_position -= glm::vec4(upDirection, 0.0);
	_target -= glm::vec4(upDirection, 0.0);
}

void Camera::moveCameraDown()
{
	glm::vec3 direction = _target - _position;
	glm::vec3 slide_vector = glm::normalize(glm::cross(direction, Constants::VEC_UP_3));
	glm::vec3 upDirection = glm::cross(direction, slide_vector) * _translationSpeed;
	_position += glm::vec4(upDirection, 0.0);
	_target += glm::vec4(upDirection, 0.0);
}