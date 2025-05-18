#pragma once

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "../../components/text.hpp"

namespace bden::systems
{

    using namespace components;

    template <typename WorldPolicy>
    class UIManager final
    {
        const snek::world<WorldPolicy> &world;

    public:
        UIManager(const snek::world<WorldPolicy> &w) : world(w) {};

        void render() const
        {
            if constexpr (WorldPolicy::template is_valid_component<TextComponent>())
            {
                auto view = world.template view<TextComponent>();
                view.for_each([](const TextComponent &txt)
                              { DrawText(txt.text.c_str(), txt.transform.translation.x, txt.transform.translation.y, txt.font_size, txt.color); });
            }
        }
    };
}