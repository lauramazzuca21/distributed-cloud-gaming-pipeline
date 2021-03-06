#pragma once

#include "../../graphics/Model.hpp"

class Dragon : public Model {

public:

    Dragon(std::string name, Constants::MaterialType material = Constants::MaterialType::EMERALD, Constants::ShadingType shader = Constants::ShadingType::PHONG);
    void draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection);
};