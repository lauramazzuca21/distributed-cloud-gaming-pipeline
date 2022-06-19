#pragma once

#include "../../state/GameObject.hpp"

class Dragon : public GameObject {

    public:
        void update(const Constants::Input& input, double dt) override;
};