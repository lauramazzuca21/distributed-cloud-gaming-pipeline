#include "GameObject.h"
#include "../constants/Maps.h"

void GameObject::rotateOCS(Constants::VectorType rotationVector, float angle)
{
    _attributes._M = glm::rotate(_attributes._M, glm::radians(angle), Constants::axis_3[rotationVector]);
}

void GameObject::scaleOCS(glm::vec3 scaleFactor) {
    _attributes._scale = glm::vec3(scaleFactor);
    _attributes._M = glm::scale(_attributes._M, _attributes._scale);
}

void GameObject::translateOCS(glm::vec3 translationVector) {
    _attributes._M = glm::translate(_attributes._M, translationVector);
}

void GameObject::rotateWCS(Constants::VectorType rotationVector, float angle)
{
    glm::mat4 currentM = _attributes._M;
    glm::mat4 inverseAxisM = glm::inverse(_attributes._M);
    //transform my object Model matrix in world basis which is I=MM^-1
    _attributes._M *= inverseAxisM;
    //make modifications
    rotateOCS(rotationVector, angle);
    //transform coord system back to OCS
    _attributes._M *= currentM;
}

void GameObject::scaleWCS(glm::vec3 scaleFactor) {
    glm::mat4 currentM = _attributes._M;
    glm::mat4 inverseAxisM = glm::inverse(_attributes._M);
    //transform my object Model matrix in world basis which is I=MM^-1
    _attributes._M *= inverseAxisM;
    scaleOCS(scaleFactor);
    _attributes._M *= currentM;
}

void GameObject::translateWCS(glm::vec3 translationVector) {
    glm::mat4 currentM = _attributes._M;
    glm::mat4 inverseAxisM = glm::inverse(_attributes._M);
    //transform my object Model matrix in world basis which is I=MM^-1
    _attributes._M *= inverseAxisM;
    translateOCS(translationVector);
    _attributes._M *= currentM;
}