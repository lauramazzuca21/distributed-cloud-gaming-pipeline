#include "GameScene.hpp"

const FrameParams& GameScene::update(const Input& input, float dt) {
     
     _currentParams._frame_num++;
     _currentParams._scene_objects.clear();

    for(GameObject* o : _sceneObjects)
    {
        o->update(input, dt);
        _currentParams._scene_objects.push_back(o->getParams());
    }   

    for(Light* l : _sceneLights)
        l->update(input, dt);

    moveCamera(input);

    _currentParams._camera_params._projectionMatrix = _camera.getProjectionMatrix(width, height);
    _currentParams._camera_params._viewMatrix = _camera.getViewMatrix();

    return _currentParams;

}

std::vector<GameObjectParams> GameScene::getGameObjectsParams() const {
    std::vector<GameObjectParams> result;
    for(GameObject* o : _sceneObjects)
        result.push_back(o->getParams());
    return result;
}

void GameScene::addSceneObject(GameObject* obj) {
    _sceneObjects.push_back(obj);
}

void GameScene::addLightObject(Light* light) {
    _sceneLights.push_back(light);
}

void  GameScene::moveCamera(const Input& input) {
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