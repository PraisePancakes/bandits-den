#pragma once
#include "raylib.h"

namespace bden::gamelayer::components
{
    struct RigidBodyComponent
    {
        Transform transform;
        Vector2 velocity;
        float collision_radius;
        RigidBodyComponent() = default;
        RigidBodyComponent(Transform t, Vector2 v, float rad) : transform(t), velocity(v), collision_radius(rad) {};
        ~RigidBodyComponent() = default;
    };

}