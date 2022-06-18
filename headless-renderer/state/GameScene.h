#pragma once

#include <vector>
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"

class GameScene {
private:
    std::vector<GameObject*> _sceneObjects;
    // InputHandler* _handler;
    Camera _camera;
    std::vector<Light*> _sceneLights;

public:
    // GameScene(InputHandler* handler) : _handler{handler} {}

    void update(const Constants::Input& input, float dt);

    void addSceneObject(GameObject* obj);
    void addLightObject(Light* light);
    //should it return a vector of attributes or should it grant access to the GameObjects and let it retrieve them on its own?
    std::vector<Constants::GameObjectAttrbutes> getGameObjectsAttributes() const;
    std::vector<GameObject*> getSceneObjects() const {    return _sceneObjects;  }
};