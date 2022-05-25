#include "GameObject.h"
#include "../constants/Maps.h"

void GameObject::rotateOCS(Constants::VectorType rotationVector, float angle)
{
    m_attributes._M = glm::rotate(m_attributes._M, glm::radians(angle), Constants::axis_3[rotationVector]);
}

void GameObject::scaleOCS(glm::vec3 scaleFactor) {
    m_attributes._scale = glm::vec3(scaleFactor);
    m_attributes._M = glm::scale(m_attributes._M, m_attributes._scale);
}

void GameObject::translateOCS(glm::vec3 translationVector) {
    m_attributes._M = glm::translate(m_attributes._M, translationVector);
}

void GameObject::rotateWCS(Constants::VectorType rotationVector, float angle)
{
    glm::mat4 currentM = m_attributes._M;
    glm::mat4 inverseAxisM = glm::inverse(m_attributes._M);
    //transform my object Model matrix in world basis which is I=MM^-1
    m_attributes._M *= inverseAxisM;
    //make modifications
    rotateOCS(rotationVector, angle);
    //transform coord system back to OCS
    m_attributes._M *= currentM;
}

void GameObject::scaleWCS(glm::vec3 scaleFactor) {
    glm::mat4 currentM = m_attributes._M;
    glm::mat4 inverseAxisM = glm::inverse(m_attributes._M);
    //transform my object Model matrix in world basis which is I=MM^-1
    m_attributes._M *= inverseAxisM;
    scaleOCS(scaleFactor);
    m_attributes._M *= currentM;
}

void GameObject::translateWCS(glm::vec3 translationVector) {
    glm::mat4 currentM = m_attributes._M;
    glm::mat4 inverseAxisM = glm::inverse(m_attributes._M);
    //transform my object Model matrix in world basis which is I=MM^-1
    m_attributes._M *= inverseAxisM;
    translateOCS(translationVector);
    m_attributes._M *= currentM;
}