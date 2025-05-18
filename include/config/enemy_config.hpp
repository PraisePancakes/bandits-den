#pragma once
#include <iostream>
#include "raylib.h"

namespace bden::config
{

    class EnemyConfig
    {
    private:
        EnemyConfig() {};
        ~EnemyConfig() {};

    public:
        constexpr static float ENEMY_SPEED{150.f};
        constexpr static Color ENEMY_GLOW{253, 243, 214, 100};
        constexpr static Color ENEMY_COLOR{RED};
    };

}