#include "GameLoop.h"

void GameLoop::update(const Constants::Input& input, float dt) {
    for(GameObject* o : _sceneObjects)
        o->update(dt);
}
