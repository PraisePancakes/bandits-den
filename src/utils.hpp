#pragma once
#include <iostream>
#include "components/rigidbody.hpp"

namespace bden::utils
{
    static bool collided(const components::RigidBodyComponent &rb1, const components::RigidBodyComponent &rb2, float radius)
    {
        const float xsquared = (rb1.transform.translation.x - rb2.transform.translation.x) * (rb1.transform.translation.x - rb2.transform.translation.x);
        const float ysquared = (rb1.transform.translation.y - rb2.transform.translation.y) * (rb1.transform.translation.y - rb2.transform.translation.y);
        const float dist = std::sqrt(xsquared + ysquared);
        return dist < rb2.collision_radius + radius;
    }

    static bool collided(const components::RigidBodyComponent &rb1, const components::RigidBodyComponent &rb2)
    {
        const float xsquared = (rb1.transform.translation.x - rb2.transform.translation.x) * (rb1.transform.translation.x - rb2.transform.translation.x);
        const float ysquared = (rb1.transform.translation.y - rb2.transform.translation.y) * (rb1.transform.translation.y - rb2.transform.translation.y);
        const float dist = std::sqrt(xsquared + ysquared);
        return dist < rb2.collision_radius + rb1.collision_radius;
    }

    static Color get_health_color(float hp)
    {
        if (hp >= 50 && hp < 75)
            return YELLOW;
        else if (hp < 50 && hp >= 0)
            return RED;
        return GREEN;
    };
}