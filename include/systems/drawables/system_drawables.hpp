#pragma once
#include "../../internal.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

namespace bden::gamelayer::systems
{
    using namespace internal;
    using namespace components;

    template <typename WorldPolicy>
    class DrawableManager final
    {
        snek::world<WorldPolicy> &world;
        void system_drawables_shapes()
        {
            auto drawable_entities = world.template view<SquareComponent, CircleComponent>();
            drawable_entities.for_each([this](WorldPolicy::entity_type e, SquareComponent &sc, CircleComponent &cc)
                                       {
                                      
                                   // DrawRectangle(c.x, c.y, c.width, c.height, c.color);
                                   if(!world.contains(e)) return;
                                   DrawCircleGradient(sc.rect.x, sc.rect.y, cc.radius, WHITE, cc.color);
                                   DrawRectanglePro({sc.rect.x, sc.rect.y, sc.rect.width, sc.rect.height}, {sc.rect.width / 2, sc.rect.height / 2}, sc.ang, sc.color); });
        };

    public:
        DrawableManager(snek::world<WorldPolicy> &w) : world(w) {};

        void render(WorldPolicy::entity_type player)
        {
            system_drawables_shapes();
        }
    };
}