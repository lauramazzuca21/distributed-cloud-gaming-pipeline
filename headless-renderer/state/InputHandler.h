#pragma once

#include <stdlib.h>
#include <string>
#include <glm/glm.hpp>
#include "../constants/Enums.h"
#include "../constants/Structs.h"

class InputHandler {
private:
    
public:
    InputHandler();

    const Constants::Input& getInput();
};