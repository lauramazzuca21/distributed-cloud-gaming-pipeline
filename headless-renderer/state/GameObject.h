#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <stdlib.h>
#include <string>
#include <glm/glm.hpp>
#include "../constants/Enums.h"
#include "../constants/Structs.h"

class GameObject {
private:
    Constants::GameObjectAttrbutes m_attributes;

public:
    GameObject(std::string filePath, std::string name, 
        Constants::ShadingType shader = Constants::ShadingType::PASS_THROUGH, 
        Constants::MaterialType material = Constants::MaterialType::NO_MATERIAL);
    virtual void update(double dt) = 0;
    
    const std::string& getName() const {return m_attributes._name; }
    Constants::ShadingType getShaderType() const { return m_attributes._shader; }
    void setShaderType(Constants::ShadingType shader) { m_attributes._shader = shader; }
    Constants::MaterialType getMaterialType() const { return m_attributes._material; }
    void setMaterial(Constants::MaterialType material) { m_attributes._material = material; }

    glm::vec3 getScale() const { return m_attributes._scale; }
    glm::vec3 getRotAngles() const { return m_attributes._rotAngles; }
    glm::vec3 getPosition() const { return glm::vec3(m_attributes._M[3].x, m_attributes._M[3].y, m_attributes._M[3].z); }
    Constants::GameObjectAttrbutes getAttributes() const {return m_attributes; }

    void rotateOCS(Constants::VectorType rotation_vector, float angle);
    void scaleOCS(glm::vec3 scale_factor);
    void translateOCS(glm::vec3 translation_vector);
    void rotateWCS(Constants::VectorType rotation_vector, float angle);
    void scaleWCS(glm::vec3 scale_factor);
    void translateWCS(glm::vec3 translation_vector);
};

#endif