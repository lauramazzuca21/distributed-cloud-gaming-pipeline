#pragma once

#include <vector>

#include "GameObject.hpp"
#include "Camera.hpp"
#include "Light.hpp"

#include "../constants/Types.hpp"

class GameScene {
private:
    static const int	width = 1280;
    static const int	height = 720;

    std::vector<GameObject*> _sceneObjects;
    Camera _camera;
    std::vector<Light*> _sceneLights;
    FrameParams _currentParams;
    void  moveCamera(const Input& input);
public:
    const FrameParams& update(const Input& input, float dt);

    void addSceneObject(GameObject* obj);
    void addLightObject(Light* light);
    //should it return a vector of attributes or should it grant access to the GameObjects and let it retrieve them on its own?
    std::vector<GameObjectParams> getGameObjectsParams() const;
    std::vector<GameObject*> getSceneObjects() const {    return _sceneObjects;  }
};