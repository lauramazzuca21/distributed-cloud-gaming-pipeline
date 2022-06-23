#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../extern/uuid/uuid_v4.h"
#include "../../extern/cborg/cborg/Cbor.h"
#include "../../utils/LogUtils.hpp"

#include "../Enums.hpp"

namespace Structs {

   struct _GameObjectParams {

        std::string _UUID;
        std::string _mesh; //change to mesh_id:[string|enum]
        Constants::ShadingType _shader; 
        Constants::MaterialType _material; 
        
        glm::mat4 _M;

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
                        .array();
                    //FIXME:: cborg doesn't handle well float encoding, this is a temp fix - better implement float encoding
                    for(int i = 0; i < 16; i++)
                    {
                        int32_t value;
                        memcpy(&value,&fM[i], sizeof(float) );
                        encoder
                            .item(value);
                    }
                    encoder
                        .end()
                    .end();

            return encoder.getLength();            
        }


        // std::size_t decodeCBOR(By) {
        //     Cborg decoder(buffer.data(), buffer.size());

        //     return decoder.getCBORLength();
        // }
    };

}