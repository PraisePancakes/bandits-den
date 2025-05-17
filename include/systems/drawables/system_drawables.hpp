#pragma once
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "../../config.hpp"
#include "../../utils.hpp"

namespace bden::systems
{

    using namespace components;

    template <typename WorldPolicy>
    class DrawableManager final
    {
        const snek::world<WorldPolicy> &world;
        void system_drawables_shapes()
        {

            auto drawable_entities = world.template view<SquareComponent, CircleComponent>();
            drawable_entities.for_each([this](WorldPolicy::entity_type e, const SquareComponent &sc, const CircleComponent &cc)
                                       {
                                      
                                   // DrawRectangle(c.x, c.y, c.width, c.height, c.color);
                                   if(!world.contains(e)) return;
                                  
                                   DrawCircleGradient(sc.rect.x, sc.rect.y, cc.radius, WHITE, cc.color);
                                   DrawRectanglePro({sc.rect.x, sc.rect.y, sc.rect.width, sc.rect.height}, {sc.rect.width / 2, sc.rect.height / 2}, sc.ang, sc.color); });
        }

        void system_drawables_weapons()
        {
            using namespace config::ui;

            auto drawable_weapons = world.template view<RigidBodyComponent, WeaponComponent>();
            drawable_weapons.for_each([this](RigidBodyComponent &rb, WeaponComponent &wc)
                                      { Vector2 center = {rb.transform.translation.x, rb.transform.translation.y};
                                       
                                       
                                        DrawCircleSector(center, wc.radius,weapon_animations::START_ANGLE, weapon_animations::END_ANGLE, weapon_animations::NSEGS, wc.radius_color); });
        };

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
        }

        void system_drawables_particles()
        {
            auto drawable_entities = world.template view<ParticleComponent>();
            drawable_entities.for_each([this](WorldPolicy::entity_type e, const ParticleComponent &particle)
                                       {
                                   if(!world.contains(e)) return;
                                   DrawCircleGradient(particle.rigidbody.transform.translation.x, particle.rigidbody.transform.translation.y, particle.rigidbody.collision_radius, WHITE, particle.color); });
        }

    public:
        DrawableManager(const snek::world<WorldPolicy> &w) : world(w) {};

        void render()
        {
            if constexpr (WorldPolicy::template is_valid_component_set<SquareComponent, CircleComponent>())
            {
                system_drawables_shapes();
            };
            if constexpr (WorldPolicy::template is_valid_component_set<RigidBodyComponent, WeaponComponent>())
            {
                system_drawables_weapons();
            }
            if constexpr (WorldPolicy::template is_valid_component<HealthComponent>())
            {
                system_drawables_health();
            };

            if constexpr (WorldPolicy::template is_valid_component<ParticleComponent>())
            {
                system_drawables_particles();
            }
        }
    };
}