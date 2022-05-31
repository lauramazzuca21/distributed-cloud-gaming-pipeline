#include "GameLoop.h"

void GameLoop::update(float dt) {
    for(GameObject* o : _sceneObjects)
        o->update(dt);
}
