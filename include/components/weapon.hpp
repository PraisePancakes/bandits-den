#pragma once
#include "raylib.h"

namespace bden::components
{
    struct WeaponComponent
    {
        float radius;
        float damage;
        float speed;
        Color radius_color;

        WeaponComponent() = default;
        WeaponComponent(float r, float d, float s, Color rc) : radius(r), damage(d), speed(s), radius_color(rc) {};
        ~WeaponComponent() = default;
    };

}