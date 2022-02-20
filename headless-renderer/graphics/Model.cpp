#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>

Model::Model(std::string filePath, std::string name, Constants::ShadingType shader, Constants::MaterialType material) : _name{name}, _shader{shader} {
    _material = Constants::Material(material);
    _mesh = new Mesh(filePath);

    _M = glm::mat4(1.0);
    _M = glm::translate(_M, glm::vec3(0.0f));
    _M = glm::rotate(_M, glm::radians(_rotAngles.x), Constants::axisVectors.at(Constants::VectorType::X)); 
    _M = glm::rotate(_M, glm::radians(_rotAngles.y), Constants::axisVectors.at(Constants::VectorType::Y));
    _M = glm::rotate(_M, glm::radians(_rotAngles.z), Constants::axisVectors.at(Constants::VectorType::Z));
    //scale factors are applied to the model
    _M = glm::scale(_M, _scale);
}

void Model::draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection) {
    
    shaderProgram->setUniformMatrix4("P_Matrix_pointer", projection);
    shaderProgram->setUniformMatrix4("V_Matrix_pointer", view);
    shaderProgram->setUniformMatrix4("M_Matrix_pointer", _M);

    shaderProgram->setUniformVector3("material_diffuse", getMaterial().diffuse);
    shaderProgram->setUniformVector3("material_ambient", getMaterial().ambient);
    shaderProgram->setUniformVector3("material_specular", getMaterial().specular);
    shaderProgram->setUniformFloat("material_shininess", getMaterial().shininess);

    _mesh->draw();
}

void Model::rotateOCS(Constants::VectorType rotationVector, GLfloat angle)
{
    _M = glm::rotate(_M, glm::radians(angle), Constants::axisVectors.at(rotationVector));
}

void Model::scaleOCS(glm::vec3 scaleFactor) {
    _scale = glm::vec3(scaleFactor);
    _M = glm::scale(_M, _scale);
}

void Model::translateOCS(glm::vec3 translationVector) {
    _M = glm::translate(_M, translationVector);
}

void Model::rotateWCS(Constants::VectorType rotationVector, GLfloat angle)
{
    glm::mat4 currentM = _M;
    glm::mat4 inverseAxisM = glm::inverse(_M);
    //transform my object Model matrix in world basis which is I=MM^-1
    _M *= inverseAxisM;
    //make modifications
    rotateOCS(rotationVector, angle);
    //transform coord system back to OCS
    _M *= currentM;
}

void Model::scaleWCS(glm::vec3 scaleFactor) {
    glm::mat4 currentM = _M;
    glm::mat4 inverseAxisM = glm::inverse(_M);
    //transform my object Model matrix in world basis which is I=MM^-1
    _M *= inverseAxisM;
    scaleOCS(scaleFactor);
    _M *= currentM;
}

void Model::translateWCS(glm::vec3 translationVector) {
    glm::mat4 currentM = _M;
    glm::mat4 inverseAxisM = glm::inverse(_M);
    //transform my object Model matrix in world basis which is I=MM^-1
    _M *= inverseAxisM;
    translateOCS(translationVector);
    _M *= currentM;
}


