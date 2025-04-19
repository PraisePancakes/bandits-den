#pragma once
#include "raylib.h"

namespace bden::layer::components
{
    class SquareComponent
    {
        int width;
        int height;
        int x;
        int y;
        Color color;
        SquareComponent() = default;
        SquareComponent(int w, int h, int x, int y, Color c) : width(w), height(h), x(x), y(y), color(c) {};
        ~SquareComponent() = default;
    };
}