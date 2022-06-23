#pragma once

#include <string>
#include <vector>
#include <iomanip> //for std::setprecision

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

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

        _GameObjectParams(const Cborg& decoder, uint32_t idx) {
            decoder.at(idx).find("UUID").getString(_UUID);
            decoder.at(idx).find("mesh").getString(_mesh);
            decoder.at(idx).find("shader").getUnsigned((uint32_t*)&_shader);
            decoder.at(idx).find("material").getUnsigned((uint32_t*)&_material);

            uint32_t Mparams = decoder.at(idx).find("M").getSize();
            if (Mparams != 16)
                logger::printErr("The decoder found %d params for the M matrix\n", Mparams);

            float *fM = glm::value_ptr(_M);
            for (uint32_t i = 0; i < Mparams; i++)
            {
                std::string value;
                decoder.at(idx).find("M").at(i).getString(value);
                fM[i] = std::stof(value.c_str());
            }
        }

        std::size_t encodeCBOR(Cbore& encoder) {
            float *fM = glm::value_ptr(_M);
            encoder.map(5)
                    .key("UUID").value(_UUID.c_str(), _UUID.length())
                    .key("mesh").value(_mesh.c_str(), _mesh.length())
                    .key("shader").value(_shader)
                    .key("material").value(_material)
                    .key("M")
                        .array(16);
                    //FIXME:: cborg doesn't handle well float encoding, this is a temp fix - better implement float encoding

                    for(int i = 0; i < 16; i++)
                    {
                        std::string s = std::to_string(fM[i]);
                        encoder
                            .item(s.c_str(), s.length());
                    }

            return encoder.getLength();            
        }
    };

}