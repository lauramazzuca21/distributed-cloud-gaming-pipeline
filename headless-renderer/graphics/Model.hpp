#ifndef _MODEL_H_
#define _MODEL_H_

#include "Mesh.hpp"
#include "ShaderProgram.hpp"
#include "../constants/Enums.hpp"
#include "../constants/Structs.hpp"

class Model {
    //these will be bound in the renderer, the model should become the GameObject state holder
    Mesh * _mesh; //change to mesh_id:[string|enum]
    Constants::ShadingType _shader; 
    Constants::Material _material; 

    std::string _name; //this should be the id to ref the remote model
    
    glm::mat4 _M;

    glm::vec3 _scale = glm::vec3(1.0f);
    glm::vec3 _rotAngles = glm::vec3(0.0f);

public:
    Model(std::string filePath, std::string name, 
        Constants::ShadingType shader = Constants::ShadingType::PASS_THROUGH, 
        Constants::MaterialType material = Constants::MaterialType::NO_MATERIAL);
    void draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection); //this method should be added only in the model built by the renderer
    
    const std::string& getName() const {return _name; }
    Constants::ShadingType getShaderType() { return _shader; }
    void setShaderType(Constants::ShadingType shader) { _shader = shader; }
    Constants::Material getMaterial() { return _material; }
    void setMaterial(Constants::MaterialType material) { _material = material; }

    glm::vec3 getScale() { return _scale; }
    glm::vec3 getRotAngles() { return _rotAngles; }
    glm::vec3 getPosition() { return glm::vec3(_M[3].x, _M[3].y, _M[3].z); }

    //these should not appear in this model anymore, only in remote state model
    void rotateOCS(Constants::VectorType rotation_vector, GLfloat angle);
    void scaleOCS(glm::vec3 scale_factor);
    void translateOCS(glm::vec3 translation_vector);
    void rotateWCS(Constants::VectorType rotation_vector, GLfloat angle);
    void scaleWCS(glm::vec3 scale_factor);
    void translateWCS(glm::vec3 translation_vector);
};

#endif