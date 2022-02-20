#include "Dragon.h"

Dragon::Dragon(std::string name, Constants::ShadingType shader, 
        Constants::MaterialType material) : Model::Model("meshes/xyzrgb_dragon.obj", name, shader, material) {
        scaleOCS(glm::vec3(0.05f));
}

void Dragon::draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection) {
    
    rotateOCS(Constants::VectorType::Y, 0.1f);

    Model::draw(shaderProgram, view, projection);
    
}