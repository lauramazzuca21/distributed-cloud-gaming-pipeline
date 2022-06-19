#pragma once
#include <stdlib.h>
#include <string>
#include <glm/glm.hpp>
#include "../constants/Enums.hpp"
#include "../constants/Structs.hpp"

class GameObject {
private:
    Constants::GameObjectAttrbutes _attributes;

public:
    GameObject(Constants::GameObjectAttrbutes attributes) : _attributes(attributes) {}
    GameObject() {}
    virtual void update(const Constants::Input& input, double dt) = 0;
    
    const std::string& getUUID() const {return _attributes._UUID; }
    Constants::ShadingType getShaderType() const { return _attributes._shader; }
    void setShaderType(Constants::ShadingType shader) { _attributes._shader = shader; }
    Constants::MaterialType getMaterialType() const { return _attributes._material; }
    void setMaterial(Constants::MaterialType material) { _attributes._material = material; }

    glm::vec3 getScale() const { return _attributes._scale; }
    glm::vec3 getRotAngles() const { return _attributes._rotAngles; }
    glm::vec3 getPosition() const { return glm::vec3(_attributes._M[3].x, _attributes._M[3].y, _attributes._M[3].z); }
    Constants::GameObjectAttrbutes getAttributes() const {return _attributes; }

    void rotateOCS(Constants::VectorType rotation_vector, float angle);
    void scaleOCS(glm::vec3 scale_factor);
    void translateOCS(glm::vec3 translation_vector);

    void rotateWCS(Constants::VectorType rotation_vector, float angle);
    void scaleWCS(glm::vec3 scale_factor);
    void translateWCS(glm::vec3 translation_vector);
};
