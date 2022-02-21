#include "Dragon.h"

Dragon::Dragon(std::string name, Constants::MaterialType material, Constants::ShadingType shader) 
    : Model::Model("meshes/xyzrgb_dragon.obj", name, shader, material) {
}

void Dragon::draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection) {
    
    rotateWCS(Constants::VectorType::Y, 0.1f);

    Model::draw(shaderProgram, view, projection);
    
}