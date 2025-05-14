#pragma once
#include "raylib.h"

namespace  bden::components
{
    struct HealthComponent
    {
        int hit_points;
        Rectangle health_bar;
        HealthComponent() = default;
        HealthComponent(int hp, Rectangle hb) : hit_points(hp), health_bar(hb) {};
        ~HealthComponent() = default;
    };

}