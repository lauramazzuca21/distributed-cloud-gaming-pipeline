#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm/glm.hpp>
#include <GL/glew.h>

class Light {
    private:
        glm::vec3 position = glm::vec3(5.0f, 5.0f, 25.0f);
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        GLfloat power = 1.0f;

    public:
        glm::vec3 getPosition() {return position; }
        glm::vec3 getColor() {return color; }
        GLfloat getPower() {return power; }
};

#endif