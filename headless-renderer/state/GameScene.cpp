#include "GameScene.h"

void GameScene::update(const Constants::Input& input, float dt) {
    for(GameObject* o : _sceneObjects)
        o->update(input, dt);

}

std::vector<Constants::GameObjectAttrbutes> GameScene::getGameObjectsAttributes() const {
    std::vector<Constants::GameObjectAttrbutes> result;
    for(GameObject* o : _sceneObjects)
        result.push_back(o->getAttributes());
}
