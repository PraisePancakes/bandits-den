#pragma once
#include <iostream>
#include "raylib.h"

namespace bden::config
{

    class UIConfig
    {
    private:
        UIConfig() {};
        ~UIConfig() {};

    public:
        struct WeaponAnimations
        {
            constexpr static float START_ANGLE{0.f};
            constexpr static float END_ANGLE{360.f};
            constexpr static int NSEGS{10};
        };
    };

}