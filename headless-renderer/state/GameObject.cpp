#include "GameObject.h"
#include "../constants/Maps.h"

void GameObject::rotateOCS(Constants::VectorType rotationVector, float angle)
{
    _M = glm::rotate(_M, glm::radians(angle), Constants::axis_3[rotationVector]);
}

void GameObject::scaleOCS(glm::vec3 scaleFactor) {
    _scale = glm::vec3(scaleFactor);
    _M = glm::scale(_M, _scale);
}

void GameObject::translateOCS(glm::vec3 translationVector) {
    _M = glm::translate(_M, translationVector);
}

void GameObject::rotateWCS(Constants::VectorType rotationVector, float angle)
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

void GameObject::scaleWCS(glm::vec3 scaleFactor) {
    glm::mat4 currentM = _M;
    glm::mat4 inverseAxisM = glm::inverse(_M);
    //transform my object Model matrix in world basis which is I=MM^-1
    _M *= inverseAxisM;
    scaleOCS(scaleFactor);
    _M *= currentM;
}

void GameObject::translateWCS(glm::vec3 translationVector) {
    glm::mat4 currentM = _M;
    glm::mat4 inverseAxisM = glm::inverse(_M);
    //transform my object Model matrix in world basis which is I=MM^-1
    _M *= inverseAxisM;
    translateOCS(translationVector);
    _M *= currentM;
}