#pragma once

#include "../../state/GameObject.hpp"

class Dragon : public GameObject {

    public:
        Dragon() {
            scaleOCS(glm::vec3(0.05f));
            translateOCS(glm::vec3(250.0f, 0.0f, -150.0f));
        }
        void update(const Input& input, double dt) override;
};