#pragma once

#include <vector>

#include "GameObject.hpp"
#include "Camera.hpp"
#include "Light.hpp"

#include "../constants/structs/FrameParams.hpp"
#include "../constants/structs/Input.hpp"

class GameScene {
private:
    std::vector<GameObject*> _sceneObjects;
    Camera _camera;
    std::vector<Light*> _sceneLights;

    void  moveCamera(const Structs::Input& input);
public:
    Structs::FrameParams& update(const Structs::Input& input, float dt);

    void addSceneObject(GameObject* obj);
    void addLightObject(Light* light);
    //should it return a vector of attributes or should it grant access to the GameObjects and let it retrieve them on its own?
    std::vector<Structs::GameObjectAttrbutes> getGameObjectsAttributes() const;
    std::vector<GameObject*> getSceneObjects() const {    return _sceneObjects;  }
};