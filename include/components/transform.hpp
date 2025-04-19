#pragma once
#include "raylib.h"
namespace bden::gamelayer::components
{
    struct TransformComponent
    {
        Vector2 pos;
        Vector2 vel;
        TransformComponent(Vector2 pos, Vector2 vel) : pos(pos), vel(vel) {};
        ~TransformComponent() {};
    };

}