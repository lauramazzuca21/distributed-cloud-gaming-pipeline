#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <GL/glew.h>

class Camera {
    private:
    static const int fov = 45;
    static constexpr float	zNear = 0.01f;
    static constexpr float	zFar = 10000.0f;

    glm::vec3 _position = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 _cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

    public:
        glm::vec3 getPosition() {return _position; }
        glm::mat4 getProjectionMatrix(int width, int height);
        glm::mat4 getViewMatrix();
};

#endif