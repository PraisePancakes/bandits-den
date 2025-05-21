#pragma once
#include <iostream>
#include "raylib.h"
namespace bden::components
{
    struct CircleComponent
    {
        float radius;
        Color color;

        CircleComponent() = default;
        CircleComponent(float r, Color c) : radius(r), color(c) {};
        ~CircleComponent() = default;
    };
};