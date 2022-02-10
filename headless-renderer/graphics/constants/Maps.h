#ifndef _MAPS_H_
#define _MAPS_H_

#include <map>
#include <string>

#include "Enums.h"

namespace Constants {
    static const std::map<ShadingType, std::pair<std::string, std::string>> shaderFiles = {
        {ShadingType::BLINN, {"shaders/v_blinn.glsl","shaders/f_blinn.glsl"}},
        {ShadingType::GOURAUD, {"shaders/v_gouraud.glsl","shaders/f_gouraud.glsl"}},
        {ShadingType::PASS_THROUGH, {"shaders/v_passthrough.glsl","shaders/f_passthrough.glsl"}},
        {ShadingType::PHONG, {"shaders/v_phong.glsl","shaders/f_phong.glsl"}},
        {ShadingType::TOON, {"shaders/v_toon.glsl","shaders/f_toon.glsl"}},
        {ShadingType::WAVE, {"shaders/v_wave.glsl","shaders/f_wave.glsl"}}
    };

    static const std::map<VectorType, glm::vec3> axisVectors = {
        {VectorType::X, glm::vec3(1.0f, 0.0f, 0.0f)},
        {VectorType::Y, glm::vec3(0.0f, 1.0f, 0.0f)},
        {VectorType::Z, glm::vec3(0.0f, 0.0f, 1.0f)}
    };
}

#endif