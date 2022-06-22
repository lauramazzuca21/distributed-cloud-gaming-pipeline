#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../extern/uuid/uuid_v4.h"
#include "../../extern/cborg/cborg/Cbor.h"
#include "../Enums.hpp"

namespace Structs {

   struct _GameObjectParams {

        std::string _UUID;
        std::string _mesh; //change to mesh_id:[string|enum]
        Constants::ShadingType _shader; 
        Constants::MaterialType _material; 
        
        glm::mat4 _M;
        glm::vec3 _scale = glm::vec3(1.0f);
        glm::vec3 _rotAngles = glm::vec3(0.0f);

        _GameObjectParams() {
            UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
            UUIDv4::UUID uuid = uuidGenerator.getUUID();
            _UUID = uuid.str();
        }

        std::size_t encodeCBOR(Cbore& encoder) {
            float *fM = glm::value_ptr(_M);

            encoder.map()
                    .key("UUID").value(_UUID.c_str(), _UUID.length())
                    .key("mesh").value(_mesh.c_str(), _mesh.length())
                    .key("shader").value(_shader)
                    .key("material").value(_material)
                    .key("M")
                        .array(16)
                            .item(fM[0])
                            .item(fM[1])
                            .item(fM[2])
                            .item(fM[3])
                            .item(fM[4])
                            .item(fM[5])
                            .item(fM[6])
                            .item(fM[7])
                            .item(fM[8])
                            .item(fM[9])
                            .item(fM[10])
                            .item(fM[11])
                            .item(fM[12])
                            .item(fM[13])
                            .item(fM[14])
                            .item(fM[15])
                    .key("scale")
                        .array(3)
                            .item(_scale.x)
                            .item(_scale.y)
                            .item(_scale.z)
                    .key("rotAngles")
                        .array(3)
                            .item(_rotAngles.x)
                            .item(_rotAngles.y)
                            .item(_rotAngles.z)
                    .end();

            return encoder.getLength();            
        }


        // std::size_t decodeCBOR() {
        //     Cborg decoder(buffer.data(), buffer.size());

        //     _frame_num = decoder.getTag();

        //     return decoder.getCBORLength();
        // }
    };

}