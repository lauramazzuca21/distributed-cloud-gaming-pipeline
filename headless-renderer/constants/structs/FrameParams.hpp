#pragma once

#include <vector>

#include "../../extern/cborg/cborg/Cbor.h"

#include "../Values.hpp"
#include "../Enums.hpp"

#include "../../utils/LogUtils.hpp"

namespace Structs {

    //FrameParams is the Message unit for sending frame information from one module to the other    
    struct _FrameParams {
            u_int64_t _frame_num;
            std::vector<GameObjectParams> _scene_objects;
            CameraParams _camera_params;

            size_t encodeCBOR(ByteVector& buffer) {
                Cbore encoder(buffer.data(), Constants::FRAME_PARAMS_CBOR_LEN);
                encoder
                .array(2)
                    .item(_frame_num)
                    .array();
                
                for (auto& obj : _scene_objects)
                    obj.encodeCBOR(encoder);
                
                _camera_params.encodeCBOR(encoder);
                encoder
                    .end()
                .end();

                // encoder.print();
                // logger::print("%d\n", encoder.getLength());
                return encoder.getLength();
            }

            size_t decodeCBOR(ByteVector& buffer) {
                Cborg decoder(buffer.data(), buffer.size());

                _frame_num = decoder.getTag();

                return decoder.getCBORLength();
            }
    };


}
