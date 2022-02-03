#ifndef _MODEL_H_
#define _MODEL_H_

#include "LogUtils.h"
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
    glm::vec3 _rotAngles = glm::vec3(15.0f, -15.0f, 0.0f);

public:
    Model(std::string filePath, std::string name, 
        Constants::ShadingType shader = Constants::ShadingType::PASS_THROUGH, 
        Constants::MaterialType material = Constants::MaterialType::NO_MATERIAL);

    void draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection);
    
    Constants::Material getMaterial() { return _material; }
    const std::string& getName() const {return _name; }
    Constants::ShadingType getShaderType() { return _shader; }
    void setShaderType(Constants::ShadingType shader) { _shader = shader; }

protected: 
    void rotateOCS(glm::vec3 rotation_vector, GLfloat angle);
    void scaleOCS(glm::vec3 scale_factor);
    void translateOCS(glm::vec3 translation_vector);
    void rotateWCS(glm::vec3 rotation_vector, GLfloat angle);
    void scaleWCS(glm::vec3 scale_factor);
    void translateWCS(glm::vec3 translation_vector);
};

#endif