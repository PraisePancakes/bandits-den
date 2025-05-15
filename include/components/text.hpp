#pragma once
#include <iostream>
#include "raylib.h"

namespace bden::components
{
    struct TextComponent
    {
        Transform transform;
        std::string text;
        Color color;
        int font_size;
        TextComponent() = default;
        TextComponent(Transform tr, const std::string &t, Color c, int f) : transform(tr), text(t), color(c), font_size(f) {};
        ~TextComponent() = default;
    };
};