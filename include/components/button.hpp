#pragma once
#include "raylib.h"
#include <functional>

namespace bden::components
{
    struct ButtonComponent
    {
        Transform transform;
        std::function<void()> callback;
        ButtonComponent() = default;
        ButtonComponent(Transform &t, std::function<void()> fptr) : transform(t), callback(fptr) {};
        ~ButtonComponent() = default;
    };
};