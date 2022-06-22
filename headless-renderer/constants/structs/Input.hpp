#pragma once

#include "../Enums.hpp"
namespace Structs {

    struct Input {
        float mouse_x;
        float mouse_y;
        Constants::MouseKey m_key_pressed;
        Constants::MouseKey m_key_released;
        Constants::KeyboardKey k_key_pressed;
        Constants::KeyboardKey k_key_released;
    };
}