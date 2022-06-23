#pragma once

#include "glm/glm.hpp"

namespace Constants {
    const glm::vec3 X_AXIS_3 = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 Y_AXIS_3 = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 Z_AXIS_3 = glm::vec3(0.0f, 0.0f, 1.0f);
    const glm::vec4 X_AXIS_4 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    const glm::vec4 Y_AXIS_4 = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    const glm::vec4 Z_AXIS_4 = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

    const glm::vec3 VEC_UP_3 = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec4 VEC_UP_4 = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

    const std::size_t FRAME_PARAMS_CBOR_LEN = 10000;
}