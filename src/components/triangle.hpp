#pragma once
#include "raylib.h"
namespace  bden::components
{
    struct TriangleComponent
    {
        Vector2 point1;
        Vector2 point2;
        Vector2 point3;
        Color color;
        TriangleComponent() = default;
        TriangleComponent(Vector2 p1, Vector2 p2, Vector2 p3, Color c) : point1(p1), point2(p2), point3(p3), color(c) {};
        ~TriangleComponent() = default;
    };

}