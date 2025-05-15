#pragma once
#include "../../config.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "../../utils.hpp"

namespace bden::systems
{
    using namespace config;

    template <typename WorldPolicy>
    class PhysicsManager final
    {

        snek::world<WorldPolicy> &world;

        void system_updateables_collider(float dt, WorldPolicy::entity_type player)
        {
            auto &prb = world.template get_ref<RigidBodyComponent>(player);

            auto collideables = world.template view<RigidBodyComponent>();
            collideables.for_each([this, &player, &prb, &dt](u64 id, RigidBodyComponent &rb)
                                  {
                                    if(id != player) {
                                        auto r = rb.collision_radius;
                                        if(utils::collided(rb, prb, r)) {
                                            prb.transform.translation.x += (-prb.velocity.x * dt);
                                            prb.transform.translation.y += (-prb.velocity.y * dt);
                                            rb.transform.translation.x += (-rb.velocity.x * dt);
                                            rb.transform.translation.y += (-rb.velocity.y * dt);
                                        }
                                    } });
        };

        void system_updateables_position(float dt)
        {
            auto updateables = world.template view<SquareComponent, RigidBodyComponent>();
            updateables.for_each([&dt](SquareComponent &s, RigidBodyComponent &t)
                                 {
                                     float xvel = t.velocity.x;
                                     float yvel = t.velocity.y;
                                     t.transform.translation.x += (xvel * dt);
                                     t.transform.translation.y += (yvel * dt);
                                     s.ang = t.transform.rotation.x;
                                     s.rect.x = t.transform.translation.x;
                                     s.rect.y = t.transform.translation.y; });
        }

    public:
        PhysicsManager(snek::world<WorldPolicy> &w) : world(w) {};

        void update(float dt, WorldPolicy::entity_type player)
        {
            if constexpr (WorldPolicy::template is_valid_component_set<SquareComponent, RigidBodyComponent>())
            {
                system_updateables_position(dt);
            }
            if constexpr (WorldPolicy::template is_valid_component<RigidBodyComponent>())
            {
                system_updateables_collider(dt, player);
            }
        }
    };
}