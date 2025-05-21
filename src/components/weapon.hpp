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
        Vector2 weapon_ray;

        WeaponComponent() = default;
        WeaponComponent(float r, float d, float s, Color rc, Vector2 ray) : radius(r), damage(d), speed(s), radius_color(rc), weapon_ray(ray) {};
        ~WeaponComponent() = default;
    };

}