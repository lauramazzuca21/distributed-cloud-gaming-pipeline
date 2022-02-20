#ifndef _BUDDHA_H_
#define _BUDDHA_H_

#include "../graphics/Model.h"

class Buddha : public Model {

public:

    Buddha(std::string name, Constants::ShadingType shader = Constants::ShadingType::GOURAUD, 
        Constants::MaterialType material = Constants::MaterialType::SLATE);
    void draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection);
};

#endif