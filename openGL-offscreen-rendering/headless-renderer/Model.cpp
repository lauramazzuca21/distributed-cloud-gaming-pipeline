#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>

Model::Model(std::string filePath, std::string name, Constants::ShadingType shader, Constants::MaterialType material) : _name{name}, _shader{shader} {
    _material = Constants::Material(material);
    _mesh = new Mesh(filePath);

    _M = glm::mat4(1.0);
    _M = glm::translate(_M, glm::vec3(0.0f,   0.0f,  0.0f));
    _M = glm::rotate(_M, glm::radians(_rotAngles.x), glm::vec3(1.0f, 0.0f, 0.0f)); 
    _M = glm::rotate(_M, glm::radians(_rotAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    _M = glm::rotate(_M, glm::radians(_rotAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
    //scale factors are applied to the model
    _M = glm::scale(_M, _scale);
}

void Model::draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection) {
    
    shaderProgram->setUniformMatrix4("P_Matrix_pointer", projection);
    shaderProgram->setUniformMatrix4("V_Matrix_pointer", view);
        
    rotateOCS(glm::vec3(1.0f, 0.0f, 0.0f), _rotAngles.x);
    rotateOCS(glm::vec3(0.0f, 1.0f, 0.0f), _rotAngles.y);
    rotateOCS(glm::vec3(0.0f, 0.0f, 1.0f), _rotAngles.z);

    //Passo al Vertex Shader il puntatore alla matrice _M, che sar� associata alla variabile Uniform mat4 _M
    //all'interno del Vertex shader. Uso l'identificatio MatModel
    shaderProgram->setUniformMatrix4("M_Matrix_pointer", _M);

    shaderProgram->setUniformVector3("material_diffuse", _material.diffuse);
    shaderProgram->setUniformVector3("material_ambient", _material.ambient);
    shaderProgram->setUniformVector3("material_specular", _material.specular);
    shaderProgram->setUniformFloat("material_shininess", _material.shininess);

    _mesh->draw();

    _rotAngles.x += 1.0f;
}

void Model::rotateOCS(glm::vec3 rotation_vector, GLfloat angle)
{
    _M *= glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotation_vector);
}

void Model::scaleOCS(glm::vec3 scale_factor) {
    _M *= glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor));
}

void Model::translateOCS(glm::vec3 translation_vector) {
    _M *= glm::translate(glm::mat4(1.0f), translation_vector);
}

void Model::rotateWCS(glm::vec3 rotation_vector, GLfloat angle)
{
    glm::mat4 currentM = _M;
    glm::mat4 inverseAxisM = glm::inverse(_M);
    //transform my object Model matrix in world basis which is I=MM^-1
    _M *= inverseAxisM;
    //make modifications
    _M *= glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotation_vector);
    //transform coord system back to OCS
    _M *= currentM;
}

void Model::scaleWCS(glm::vec3 scale_factor) {
    glm::mat4 currentM = _M;
    glm::mat4 inverseAxisM = glm::inverse(_M);
    //transform my object Model matrix in world basis which is I=MM^-1
    _M *= inverseAxisM;

    _M *= glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor));

    _M *= currentM;
}

void Model::translateWCS(glm::vec3 translation_vector) {
    glm::mat4 currentM = _M;
    glm::mat4 inverseAxisM = glm::inverse(_M);
    //transform my object Model matrix in world basis which is I=MM^-1
    _M *= inverseAxisM;

    _M *= glm::translate(glm::mat4(1.0f), translation_vector);
    _M *= currentM;
}


