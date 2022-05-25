#ifndef _ENUMS_H_
#define _ENUMS_H_


namespace Constants {
    enum ShadingType {
        GOURAUD,
        PHONG,
        BLINN,
        TOON,
        PASS_THROUGH,
        WAVE
    };

    enum MaterialType {
        RED_PLASTIC,
        EMERALD,
        BRASS,
        SLATE,
        YELLOW_RUBBER,
        NO_MATERIAL
    };

    enum VectorType {
        X,
        Y,
        Z
    };
}

#endif