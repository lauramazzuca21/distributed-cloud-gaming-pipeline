#pragma once

#include <vector>

#include "../../extern/cborg/cborg/Cbor.h"

#include "../Values.hpp"
#include "../Enums.hpp"

#include "../../utils/LogUtils.hpp"

namespace Structs {

    //FrameParams is the Message unit for sending frame information from one module to the other    
    struct _FrameParams {
            u_int32_t _frame_num;
            std::vector<GameObjectParams> _scene_objects;
            CameraParams _camera_params;

            size_t encodeCBOR(ByteVector& buffer) {
                Cbore encoder(buffer.data(), Constants::FRAME_PARAMS_CBOR_LEN);
                encoder
                .array(3)
                    .item(_frame_num)
                    .array(_scene_objects.size());
                
                for (auto& obj : _scene_objects)
                    obj.encodeCBOR(encoder);
                
                _camera_params.encodeCBOR(encoder);
                encoder
                    .end()
                .end();

                encoder.print();
                // logger::print("%d\n", encoder.getLength());
                return encoder.getLength();
            }

            size_t decodeCBOR(const ByteVector& buffer) {
                Cborg decoder(buffer.data(), Constants::FRAME_PARAMS_CBOR_LEN);
                decoder.at(0).getUnsigned(&_frame_num);
                uint32_t objs = decoder.at(1).getSize();
                for (uint32_t i = 0; i < objs; i++)
                    _scene_objects.push_back(GameObjectParams(decoder.at(1), i));

                return decoder.getCBORLength();
            }
    };


}
