#pragma once

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "../../utils.hpp"
#include "../../config/game_config.hpp"

namespace bden::systems
{

    using namespace components;

    template <typename WorldPolicy>
    class HealthManager final
    {
        snek::world<WorldPolicy> &world;
        std::vector<typename WorldPolicy::entity_type> &to_delete;

        void system_updateables_health_ui()
        {
            static const auto health_bottom_space = 50;
            auto updateables = world.template view<SquareComponent, RigidBodyComponent, HealthComponent>();
            updateables.for_each([this](WorldPolicy::entity_type e, const SquareComponent &sqc, const RigidBodyComponent &rbc, HealthComponent &hc)
                                 {
                                    hc.health_bar.x = rbc.transform.translation.x - (sqc.rect.width / 2);
                                    hc.health_bar.y = rbc.transform.translation.y - (sqc.rect.height / 2) - health_bottom_space;
                                    hc.health_bar.width -= (int)hc.health_bar.width % (int)hc.hit_points ;
                                   
                if(hc.hit_points <= 1) {
                 
                       this->to_delete.push_back(e);
                }; });
        }

        void system_updateables_damage(float dt, WorldPolicy::entity_type player)
        {
            const std::vector<typename WorldPolicy::entity_type> enemies = world.template get_tagged_entities(config::GameConfig::TagEnum::TAG_ENEMIES);

            auto &prb = world.template get_ref<RigidBodyComponent>(player);

            for (const auto &e : enemies)
            {
                auto &weapon = world.template get_ref<components::WeaponComponent>(e);
                const auto &rb = world.template get_ref<components::RigidBodyComponent>(e);
                const auto r = weapon.radius;

                if (utils::collided(rb, prb, r))
                {
                    static float interval = 0;
                    if (interval >= weapon.speed)
                    {
                        world.template get_ref<HealthComponent>(player).hit_points -= weapon.damage;
                        interval = 0;
                        weapon.radius_color.a = 150;
                    }
                    if (weapon.radius_color.a >= 255)
                    {
                        weapon.radius_color.a = 150;
                    }
                    else
                    {
                        weapon.radius_color.a++;
                    }

                    interval += dt;
                }
                else
                {
                    weapon.radius_color.a = 150;
                }
            }
        };

    public:
        HealthManager(snek::world<WorldPolicy> &w, std::vector<typename WorldPolicy::entity_type> &d) : world(w), to_delete(d) {};

        void update(float dt, WorldPolicy::entity_type player)
        {

            if constexpr (WorldPolicy::template is_valid_component_set<SquareComponent, RigidBodyComponent, HealthComponent>())
            {
                system_updateables_health_ui();
            }

            if constexpr (WorldPolicy::template is_valid_component_set<WeaponComponent, RigidBodyComponent>())
            {
                system_updateables_damage(dt, player);
            }
        }
    };
}