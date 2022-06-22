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
                    .array(16)
                            .item(fP[0])
                            .item(fP[1])
                            .item(fP[2])
                            .item(fP[3])
                            .item(fP[4])
                            .item(fP[5])
                            .item(fP[6])
                            .item(fP[7])
                            .item(fP[8])
                            .item(fP[9])
                            .item(fP[10])
                            .item(fP[11])
                            .item(fP[12])
                            .item(fP[13])
                            .item(fP[14])
                            .item(fP[15])
                    .key("viewMatrix")
                        .array(16)
                            .item(fV[0])
                            .item(fV[1])
                            .item(fV[2])
                            .item(fV[3])
                            .item(fV[4])
                            .item(fV[5])
                            .item(fV[6])
                            .item(fV[7])
                            .item(fV[8])
                            .item(fV[9])
                            .item(fV[10])
                            .item(fV[11])
                            .item(fV[12])
                            .item(fV[13])
                            .item(fV[14])
                            .item(fV[15])
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
