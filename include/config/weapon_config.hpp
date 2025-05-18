#pragma once
#include "raylib.h"

namespace bden::config
{
    class WeaponConfig
    {
    private:
        WeaponConfig() {};
        ~WeaponConfig() {};

    public:
        struct spear
        {
            constexpr static float radius{1.5f};
            constexpr static Color color{RED};
            constexpr static float damage{5.f};
            constexpr static float speed{2.f};
        };
    };

};