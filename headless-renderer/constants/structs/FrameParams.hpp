#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "../extern/uuid/uuid_v4.h"
#include "../Enums.hpp"

namespace Structs {
    //FrameParams is the Message unit for sending frame information from one module to the other
    struct FrameParams {
            u_int64_t _frame_num;
            std::vector<GameObjectAttrbutes> _scene_objects;
        };

    struct GameObjectAttrbutes {

        std::string _UUID;
        std::string _mesh; //change to mesh_id:[string|enum]
        Constants::ShadingType _shader; 
        Constants::MaterialType _material; 
        
        glm::mat4 _M;
        glm::vec3 _scale = glm::vec3(1.0f);
        glm::vec3 _rotAngles = glm::vec3(0.0f);

        GameObjectAttrbutes() {
            UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
            UUIDv4::UUID uuid = uuidGenerator.getUUID();
            _UUID = uuid.str();
        }

    };

}
