#pragma once

namespace bden::gamelayer::components
{
    struct ColliderComponent
    {
        float radius;
        ColliderComponent() = default;
        ColliderComponent(float r) : radius(r) {};
        ~ColliderComponent() = default;
    };

}