#pragma once

#include "../../state/GameObject.hpp"

class Dragon : public GameObject {

    public:
        Dragon(std::string mesh = "meshes/xyzrgb_dragon.obj") : GameObject(mesh) {
            scaleOCS(glm::vec3(0.05f));
            translateOCS(glm::vec3(250.0f, 0.0f, -150.0f));  
            setMaterial(Constants::MaterialType::EMERALD);
            setShaderType(Constants::ShadingType::PHONG);
        }
        void update(const Input& input, double dt) override;
};