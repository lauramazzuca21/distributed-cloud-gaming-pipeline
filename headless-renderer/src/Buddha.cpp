#include "Buddha.h"

Buddha::Buddha(std::string name, Constants::ShadingType shader, 
        Constants::MaterialType material) : Model::Model("meshes/xyzrgb_dragon.obj", name, shader, material) {
        scaleOCS(glm::vec3(0.1f));
        translateOCS(glm::vec3(0.0f, 0.0f, 50.0f));
}

void Buddha::draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection) {
    
    rotateWCS(Constants::VectorType::Y, 0.1f);

    Model::draw(shaderProgram, view, projection);
    
}