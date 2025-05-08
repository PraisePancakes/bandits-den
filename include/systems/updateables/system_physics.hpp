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
                                        float xsquared = (rb.transform.translation.x - prb.transform.translation.x) * (rb.transform.translation.x - prb.transform.translation.x);
                                        float ysquared = (rb.transform.translation.y - prb.transform.translation.y) * (rb.transform.translation.y - prb.transform.translation.y);
                                        float dist = std::sqrt(xsquared + ysquared);
                                        bool collided = dist < prb.collision_radius + r;
                                        if(collided) {
                                            prb.transform.translation.x += (-prb.velocity.x * dt);
                                            prb.transform.translation.y += (-prb.velocity.y * dt);
                                            rb.transform.translation.x += (-rb.velocity.x * dt);
                                            rb.transform.translation.y += (-rb.velocity.y * dt);
                                            
                                           world.template get_ref<HealthComponent>(player).hit_points--;
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
            system_updateables_position(dt);
            system_updateables_collider(dt, player);
        }
    };
}