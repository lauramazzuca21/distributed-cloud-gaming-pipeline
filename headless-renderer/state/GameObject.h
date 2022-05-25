#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <stdlib.h>
#include <string>
#include <glm/glm.hpp>
#include "../constants/Enums.h"
#include "../constants/Structs.h"

class GameObject {
private:
    Constants::GameObjectAttrbutes _attributes;

public:
    GameObject(std::string filePath, std::string name, 
        Constants::ShadingType shader = Constants::ShadingType::PASS_THROUGH, 
        Constants::MaterialType material = Constants::MaterialType::NO_MATERIAL);
    virtual void update(double dt) = 0;
    
    const std::string& getName() const {return _attributes._name; }
    Constants::ShadingType getShaderType() { return _attributes._shader; }
    void setShaderType(Constants::ShadingType shader) { _attributes._shader = shader; }
    Constants::MaterialType getMaterialType() { return _attributes._material; }
    void setMaterial(Constants::MaterialType material) { _attributes._material = material; }

    glm::vec3 getScale() { return _attributes._scale; }
    glm::vec3 getRotAngles() { return _attributes._rotAngles; }
    glm::vec3 getPosition() { return glm::vec3(_attributes._M[3].x, _attributes._M[3].y, _attributes._M[3].z); }

    void rotateOCS(Constants::VectorType rotation_vector, float angle);
    void scaleOCS(glm::vec3 scale_factor);
    void translateOCS(glm::vec3 translation_vector);
    void rotateWCS(Constants::VectorType rotation_vector, float angle);
    void scaleWCS(glm::vec3 scale_factor);
    void translateWCS(glm::vec3 translation_vector);
};

#endif