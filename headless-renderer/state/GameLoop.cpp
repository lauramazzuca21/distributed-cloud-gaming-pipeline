#include "GameLoop.h"

void GameLoop::update(float dt) {
    for(GameObject* o : m_sceneObjects)
        o->update(dt);
}
