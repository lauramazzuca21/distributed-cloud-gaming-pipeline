#include "GODragon.hpp"

void Dragon::update(const Input& input, double dt){
    rotateOCS(Constants::VectorType::Y, 10.0f);    
}