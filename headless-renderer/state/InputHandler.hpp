#pragma once

#include <stdlib.h>
#include <string>
#include <glm/glm.hpp>
#include "../constants/Enums.hpp"
#include "../constants/Structs.hpp"

class InputHandler {
private:
    
public:
    InputHandler();

    const Constants::Input& getInput();
};