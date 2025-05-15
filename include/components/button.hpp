#pragma once
#include "raylib.h"
#include <functional>

namespace bden::components
{
    struct ButtonComponent
    {
        Color color;
        Transform transform;
        std::function<void()> callback;
        ButtonComponent() = default;
        ButtonComponent(Color c, Transform &t, std::function<void()> fptr) : color(c), transform(t), callback(fptr) {};
        ~ButtonComponent() = default;
    };
};