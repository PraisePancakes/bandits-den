#pragma once

namespace  bden::components
{
    struct AggroComponent
    {
        float aggro_radius;
        bool is_aggro;
        AggroComponent() = default;
        AggroComponent(float ar, bool ia) : aggro_radius(ar), is_aggro(ia) {};
        ~AggroComponent() = default;
    };
}