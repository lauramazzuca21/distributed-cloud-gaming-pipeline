#include "Dragon.hpp"

Dragon::Dragon(std::string name, Constants::MaterialType material, Constants::ShadingType shader) 
    : Model::Model("meshes/xyzrgb_dragon.obj", name, shader, material) {
}

void Dragon::draw(ShaderProgram * shaderProgram, glm::mat4 view, glm::mat4 projection) {
    Model::draw(shaderProgram, view, projection);
}
