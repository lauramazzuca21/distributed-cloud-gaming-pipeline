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

            encoder.map(2)
                    .key("P")
                        .array(16);
                        //FIXME:: cborg doesn't handle well float encoding, this is a temp fix - better implement float encoding
                    for(int i = 0; i < 16; i++)
                    {
                        std::string s = std::to_string(fP[i]);
                        encoder
                            .item(s.c_str(), s.length());
                    }
                    encoder
                        .key("V")
                            .array(16);
                    for(int i = 0; i < 16; i++)
                    {
                        std::string s = std::to_string(fV[i]);
                        encoder
                            .item(s.c_str(), s.length());
                    }

            return encoder.getLength();            
        }


        void decodeCBOR(const Cborg& decoder, uint32_t idx) {
            decoder.print();
            uint32_t Pparams = decoder.at(idx).find("P").getSize();
            uint32_t Vparams = decoder.at(idx).find("V").getSize();
            if (Pparams != 16 || Vparams != 16)
                logger::printErr("The decoder found %d params for the P matrix and %d for the V matrix\n", Pparams, Vparams);


            float *fP = glm::value_ptr(_projectionMatrix);
            for (uint32_t i = 0; i < Pparams; i++)
            {
                std::string value;
                decoder.at(idx).find("P").at(i).getString(value);
                fP[i] = std::stof(value.c_str());
            }
            float *fV = glm::value_ptr(_viewMatrix);
            for (uint32_t i = 0; i < Vparams; i++)
            {
                std::string value;
                decoder.at(idx).find("V").at(i).getString(value);
                fV[i] = std::stof(value.c_str());
            }
        }

    };
}
