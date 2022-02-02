#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>

Model::Model(std::string filePath, std::string name, Constants::ShadingType shader) : _name{name}, _shader{shader} {
    _mesh = new Mesh(filePath);
}

void Model::Draw(GLuint * MatModel) {
    static GLfloat angle = 0.0f; 

    _M = glm::mat4(1.0);
    _M = glm::translate(_M, glm::vec3(0.0f,   0.0f,  0.0f));
    _M = glm::rotate(_M, glm::radians(_rotAngles.x + angle), glm::vec3(1.0f, 0.0f, 0.0f)); 
    _M = glm::rotate(_M, glm::radians(_rotAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    _M = glm::rotate(_M, glm::radians(_rotAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
    //scale factors are applied to the model
    _M = glm::scale(_M, _scale);

    //Passo al Vertex Shader il puntatore alla matrice _M, che sar� associata alla variabile Uniform mat4 _M
    //all'interno del Vertex shader. Uso l'identificatio MatModel
    glUniformMatrix4fv((*MatModel), 1, GL_FALSE, glm::value_ptr(_M));

    _mesh->Draw();
    angle+=10.0f;
}