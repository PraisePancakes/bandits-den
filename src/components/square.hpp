#pragma once
#include "raylib.h"

namespace  bden::components
{
    struct SquareComponent
    {
        Rectangle rect;
        Color color;
        float ang;
        SquareComponent() = default;
        SquareComponent(float w, float h, float x, float y, Color c, float ang) : rect(x, y, w, h), color(c), ang(ang) {};
        ~SquareComponent() = default;
    };
}