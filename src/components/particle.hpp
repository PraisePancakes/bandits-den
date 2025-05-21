#pragma once
#include "raylib.h"
#include "rigidbody.hpp"

namespace bden::components
{
    struct ParticleComponent
    {
        RigidBodyComponent rigidbody;
        Color color;
        ParticleComponent() = default;
        ParticleComponent(RigidBodyComponent rb, Color c) : rigidbody(rb), color(c) {};
        ~ParticleComponent() = default;
    };

}