#pragma once

#include <glm/glm.hpp>

class Camera {
    private:
        float   _fov = 45;
        float	_zNear = 0.01f;
        float	_zFar = 10000.0f;

        float _zoomSpeed = 0.1f;
        float _translationSpeed = 0.01f;

        glm::vec4 _position = glm::vec4(0.0f, 0.0f, 10.0f, 0.0f);
        glm::vec4 _cameraFront = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        glm::vec4 _target = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    public:
        Camera(glm::vec4 _position = glm::vec4(0.0f, 0.0f, 10.0f, 0.0f), 
            int fov = 45, float zNear = 0.01f, float zFar = 10000.0f) 
        : _fov{45}, _zNear{zNear}, _zFar{zFar} {}

        glm::vec4 getPosition() {return _position; }
        glm::mat4 getProjectionMatrix(int width, int height);
        glm::mat4 getViewMatrix();

        void setZoomSpeed(float zoomSpeed) {_zoomSpeed = zoomSpeed; }
        void setTranslationSpeed(float translationSpeed) {_translationSpeed = translationSpeed; }

        void moveCameraForeward();
        void moveCameraBack();
        void moveCameraLeft();
        void moveCameraRight();
        void moveCameraUp();
        void moveCameraDown();
};