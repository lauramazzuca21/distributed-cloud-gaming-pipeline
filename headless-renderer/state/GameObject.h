#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <stdlib.h>
#include <string>
#include <glm/glm.hpp>
#include "../constants/Enums.h"

class GameObject {
    //these will be bound in the renderer, the model should become the GameObject state holder
    std::string _mesh; //change to mesh_id:[string|enum]
    Constants::ShadingType _shader; 
    Constants::MaterialType _material; 

    std::string _name; //this should be the id to ref the remote model
    
    glm::mat4 _M;

    glm::vec3 _scale = glm::vec3(1.0f);
    glm::vec3 _rotAngles = glm::vec3(0.0f);

public:
    GameObject(std::string filePath, std::string name, 
        Constants::ShadingType shader = Constants::ShadingType::PASS_THROUGH, 
        Constants::MaterialType material = Constants::MaterialType::NO_MATERIAL);
    virtual void update(double dt) = 0;
    
    const std::string& getName() const {return _name; }
    Constants::ShadingType getShaderType() { return _shader; }
    void setShaderType(Constants::ShadingType shader) { _shader = shader; }
    Constants::MaterialType getMaterialType() { return _material; }
    void setMaterial(Constants::MaterialType material) { _material = material; }

    glm::vec3 getScale() { return _scale; }
    glm::vec3 getRotAngles() { return _rotAngles; }
    glm::vec3 getPosition() { return glm::vec3(_M[3].x, _M[3].y, _M[3].z); }

    void rotateOCS(Constants::VectorType rotation_vector, float angle);
    void scaleOCS(glm::vec3 scale_factor);
    void translateOCS(glm::vec3 translation_vector);
    void rotateWCS(Constants::VectorType rotation_vector, float angle);
    void scaleWCS(glm::vec3 scale_factor);
    void translateWCS(glm::vec3 translation_vector);
};

#endif