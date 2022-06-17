#pragma once

#include <vector>
#include "GameObject.h"

class GameLoop {
private:
    std::vector<GameObject*> _sceneObjects;

public:
    void update(const Constants::Input& input, float dt);
    //should it return a vector of attributes or should it grant access to the GameObjects and let it retrieve them on its own?
    std::vector<Constants::GameObjectAttrbutes> getGameObjectsAttributes() const {
        std::vector<Constants::GameObjectAttrbutes> result;
        for(GameObject* o : _sceneObjects)
            result.push_back(o->getAttributes());
    }

    std::vector<GameObject*> getSceneObjects() const {    return _sceneObjects;  }

};