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
            const std::vector<typename WorldPolicy::entity_type> enemies = world.template get_tagged_entities((typename WorldPolicy::entity_type)internal::TagEnum::TAG_ENEMIES);

            auto &prb = world.template get_ref<RigidBodyComponent>(player);

            for (const auto &e : enemies)
            {

                const auto &rb = world.template get_ref<components::RigidBodyComponent>(e);
                const auto r = rb.collision_radius;

                float prev_player_x = prb.transform.translation.x + (prb.velocity.x * dt);
                float prev_player_y = prb.transform.translation.y + (prb.velocity.y * dt);
                float prev_rb_x = rb.transform.translation.x + (rb.velocity.x * dt);
                float prev_rb_y = rb.transform.translation.y + (rb.velocity.y * dt);

                const float xsquared = (prev_rb_x - prev_player_x) * (prev_rb_x - prev_player_x);
                const float ysquared = (prev_rb_y - prev_player_y) * (prev_rb_y - prev_player_y);
                const float dist = std::sqrt(xsquared + ysquared);

                const bool collided = dist < prb.collision_radius + r;

                if (collided)
                {
                    world.template get_ref<HealthComponent>(player).hit_points--;
                }
            }
        };

    public:
        HealthManager(snek::world<WorldPolicy> &w, std::vector<typename WorldPolicy::entity_type> &d) : world(w), to_delete(d) {};

        void update(float dt, WorldPolicy::entity_type player)
        {

            system_updateables_health_ui();
            system_updateables_damage(dt, player);
        }
    };
}