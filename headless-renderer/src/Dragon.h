#ifndef _DRAGON_H_
#define _DRAGON_H_

#include "../graphics/Model.h"

class Dragon : public Model {

public:

    Dragon(std::string name, Constants::ShadingType shader = Constants::ShadingType::PHONG, 
        Constants::MaterialType material = Constants::MaterialType::EMERALD);
    void draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection);
};

#endif