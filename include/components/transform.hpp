#pragma once
#include "raylib.h"
namespace bden::gamelayer::components
{
    struct TransformComponent
    {
        Vector2 pos;
        Vector2 vel;
        float ang;
        TransformComponent(Vector2 pos, Vector2 vel, float ang) : pos(pos), vel(vel), ang(ang) {};
        ~TransformComponent() {};
    };

}