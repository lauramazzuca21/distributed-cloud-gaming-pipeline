#pragma once

#include <glm/glm.hpp>

#include "../../extern/cborg/cborg/Cbor.h"

namespace Structs {

    //FrameParams is the Message unit for sending frame information from one module to the other    
    struct _CameraParams {
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix;

        std::size_t encodeCBOR(Cbore& encoder) {
            float *fP = glm::value_ptr(_projectionMatrix);
            float *fV = glm::value_ptr(_viewMatrix);

            encoder.map()
                    .key("projectionMatrix")
                        .array();
                        //FIXME:: cborg doesn't handle well float encoding, this is a temp fix - better implement float encoding
                    for(int i = 0; i < 16; i++)
                    {
                        int32_t value;
                        memcpy(&value,&fP[i], sizeof(float) );
                        encoder
                            .item(value);
                    }
                    encoder
                        .end()
                    .key("viewMatrix")
                        .array();
                    for(int i = 0; i < 16; i++)
                    {
                        int32_t value;
                        memcpy(&value,&fV[i], sizeof(float) );
                        encoder
                            .item(value);
                    }
                    encoder
                        .end()
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
