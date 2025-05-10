#pragma once
#include "../../config.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

namespace bden::gamelayer::systems
{

    using namespace components;

    template <typename WorldPolicy>
    class UIManager final
    {
        snek::world<WorldPolicy> &world;

        Color get_health_color(float hp)
        {
            if (hp >= 50 && hp < 75)
                return YELLOW;
            else if (hp < 50 && hp >= 0)
                return RED;
            return GREEN;
        };

        void system_drawables_health()
        {
            auto drawable_entities = world.template view<HealthComponent>();
            drawable_entities.for_each([this](WorldPolicy::entity_type e, const HealthComponent &hc)
                                       {
                                        if (!world.contains(e)) return;
                                        DrawRectangleLines(hc.health_bar.x, hc.health_bar.y, hc.health_bar.width + 1, hc.health_bar.height + 1, BEIGE);
                                        DrawText(TextFormat("%d", hc.hit_points), hc.health_bar.x + hc.health_bar.width + 5, hc.health_bar.y, 20, WHITE);
                                        DrawRectangleRec(hc.health_bar, get_health_color(hc.hit_points)); });
        };

    public:
        UIManager(snek::world<WorldPolicy> &w) : world(w) {};

        void render()
        {
            system_drawables_health();
        }
    };
}