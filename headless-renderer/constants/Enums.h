#pragma once
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

    enum MouseKey {
        LEFT,
        RIGHT,
        WHEEL
    };

    enum KeyboardKey {
        W,
        A,
        S,
        D
    };
}