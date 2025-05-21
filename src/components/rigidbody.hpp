#pragma once
#include "raylib.h"

namespace bden::components
{
    struct RigidBodyComponent
    {
        Transform transform;
        Vector2 velocity;
        float collision_radius;
        float speed;
        RigidBodyComponent() = default;
        RigidBodyComponent(Transform t, Vector2 v, float rad, float s) : transform(t), velocity(v), collision_radius(rad), speed(s) {};
        ~RigidBodyComponent() = default;
    };

}