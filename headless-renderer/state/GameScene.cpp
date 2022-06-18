#include "GameScene.h"

void GameScene::update(const Constants::Input& input, float dt) {
    for(GameObject* o : _sceneObjects)
        o->update(input, dt);

}
