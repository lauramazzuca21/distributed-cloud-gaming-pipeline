#pragma once

#include <glm/glm.hpp>

class Light {
    private:
        glm::vec3 position = glm::vec3(5.0f, 5.0f, 25.0f);
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        float_t power = 1.0f;

    public:
        glm::vec3 getPosition() {return position; }
        glm::vec3 getColor() {return color; }
        float_t getPower() {return power; }

        void update(const Input& input, double dt) {}
};