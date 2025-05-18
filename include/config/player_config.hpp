#pragma once
#include <iostream>
#include "raylib.h"

namespace bden::config
{

    class PlayerConfig
    {
    private:
        PlayerConfig() {};
        ~PlayerConfig() {};

    public:
        constexpr static float PLAYER_SPEED{250.f};
        constexpr static Color PLAYER_COLOR{BLUE};
        constexpr static Color PLAYER_GLOW{253, 243, 214, 100};
    };

}