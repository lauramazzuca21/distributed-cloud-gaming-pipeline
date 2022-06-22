#include "GameScene.hpp"

Constants::FrameParams& GameScene::update(const Constants::Input& input, float dt) {
     
    for(GameObject* o : _sceneObjects)
        o->update(input, dt);

    moveCamera(input);
}

std::vector<Constants::GameObjectAttrbutes> GameScene::getGameObjectsAttributes() const {
    std::vector<Constants::GameObjectAttrbutes> result;
    for(GameObject* o : _sceneObjects)
        result.push_back(o->getAttributes());
    return result;
}

void GameScene::addSceneObject(GameObject* obj) {
    _sceneObjects.push_back(obj);
}

void GameScene::addLightObject(Light* light) {
    _sceneLights.push_back(light);
}

void  GameScene::moveCamera(const Constants::Input& input) {
    switch(input.m_key_pressed)
    {
        case Constants::KeyboardKey::W:
            _camera.moveCameraUp();
            break;
        case Constants::KeyboardKey::A:
            _camera.moveCameraLeft();
            break;
        case Constants::KeyboardKey::S:
            _camera.moveCameraDown();
            break;
        case Constants::KeyboardKey::D:
            _camera.moveCameraRight();
            break;
    }

}