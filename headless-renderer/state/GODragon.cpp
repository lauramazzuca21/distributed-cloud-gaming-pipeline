#include "GODragon.h"

void Dragon::update(const Constants::Input& input, double dt){
    rotateOCS(Constants::VectorType::Y, 10.0f);    
}