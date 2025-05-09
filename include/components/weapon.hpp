#pragma once
#include "raylib.h"

namespace bden::gamelayer::components
{
    struct WeaponComponent
    {
        float radius;
        float damage;
        Color radius_color;
        WeaponComponent() = default;
        WeaponComponent(float r, float d, Color rc) : radius(r), damage(d), radius_color(rc) {};
        ~WeaponComponent() = default;
    };

}