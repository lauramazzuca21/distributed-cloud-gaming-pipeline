#pragma once

#include "../../state/GameObject.h"

class Dragon : public GameObject {

    public:
        void update(const Constants::Input& input, double dt) override;
};