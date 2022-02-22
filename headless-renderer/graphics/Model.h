#ifndef _MODEL_H_
#define _MODEL_H_

#include "Mesh.h"
#include "ShaderProgram.h"
#include "constants/Enums.h"
#include "constants/Structs.h"

class Model {
    Mesh * _mesh;
    Constants::ShadingType _shader;
    Constants::Material _material;

    std::string _name;
    
    glm::mat4 _M;

    glm::vec3 _scale = glm::vec3(1.0f);
    glm::vec3 _rotAngles = glm::vec3(0.0f);

public:
    Model(std::string filePath, std::string name, 
        Constants::ShadingType shader = Constants::ShadingType::PASS_THROUGH, 
        Constants::MaterialType material = Constants::MaterialType::NO_MATERIAL);
    virtual void update(double dt) = 0;
    void draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection);
    
    const std::string& getName() const {return _name; }
    Constants::ShadingType getShaderType() { return _shader; }
    void setShaderType(Constants::ShadingType shader) { _shader = shader; }
    Constants::Material getMaterial() { return _material; }
    void setMaterial(Constants::MaterialType material) { _material = material; }

    glm::vec3 getScale() { return _scale; }
    glm::vec3 getRotAngles() { return _rotAngles; }
    glm::vec3 getPosition() { return glm::vec3(_M[3].x, _M[3].y, _M[3].z); }


    void rotateOCS(Constants::VectorType rotation_vector, GLfloat angle);
    void scaleOCS(glm::vec3 scale_factor);
    void translateOCS(glm::vec3 translation_vector);
    void rotateWCS(Constants::VectorType rotation_vector, GLfloat angle);
    void scaleWCS(glm::vec3 scale_factor);
    void translateWCS(glm::vec3 translation_vector);
};

#endif