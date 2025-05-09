#pragma once
#include "../../internal.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "../../config.hpp"

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

        void system_drawables_weapons()
        {
            constexpr static float start_angle = 0.f;
            constexpr static float end_angle = 360.f;
            constexpr static int nsegs = 10;

            auto drawable_weapons = world.template view<RigidBodyComponent, WeaponComponent>();
            drawable_weapons.for_each([this](RigidBodyComponent &rb, WeaponComponent &wc)
                                      { Vector2 center = {rb.transform.translation.x , rb.transform.translation.y};
                                        DrawCircleSectorLines(center, wc.radius,config::ui::weapons::START_ANGLE, config::ui::weapons::END_ANGLE, config::ui::weapons::NSEGS, wc.radius_color); });
        };

    public:
        DrawableManager(snek::world<WorldPolicy> &w) : world(w) {};

        void render(WorldPolicy::entity_type player)
        {
            system_drawables_shapes();
            system_drawables_weapons();
        }
    };
}