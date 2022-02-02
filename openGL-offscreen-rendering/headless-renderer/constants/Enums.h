#ifndef _ENUMS_H_
#define _ENUMS_H_


namespace Constants {
    typedef enum {
        GOURAUD,
        PHONG,
        BLINN,
        TOON,
        PASS_THROUGH,
        WAVE
    } ShadingType;

    typedef enum {
        RED_PLASTIC,
        EMERALD,
        BRASS,
        SLATE,
        YELLOW_RUBBER,
        NO_MATERIAL
    } MaterialType;
}

#endif