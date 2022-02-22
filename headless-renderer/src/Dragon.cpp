#include "Dragon.h"

Dragon::Dragon(std::string name, Constants::MaterialType material, Constants::ShadingType shader) 
    : Model::Model("meshes/xyzrgb_dragon.obj", name, shader, material) {
}

void Dragon::update(double dt) {
    
    rotateOCS(Constants::VectorType::Y, 10.0f);    
}


void Dragon::draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection) {
    
    Model::draw(shaderProgram, view, projection);
    
}
