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

        void system_updateables_collider(float dt)
        {
            auto collideables = world.template view<RigidBodyComponent>();
            auto filtered = collideables.filtered();

            auto &ids = filtered[0];
            for (size_t i = 0; i < ids.size(); i++)
            {
                auto eid_i = ids[i];
                auto &first = world.template get_ref<RigidBodyComponent>(eid_i);
                for (size_t j = i + 1; j < ids.size(); j++)
                {
                    auto eid_j = ids[j];
                    if (eid_i == eid_j)
                        continue;
                    auto &second = world.template get_ref<RigidBodyComponent>(eid_j);

                    if (utils::collided(first, second))
                    {
                        first.transform.translation.x -= first.velocity.x * dt;
                        first.transform.translation.y -= first.velocity.y * dt;
                        second.transform.translation.x -= second.velocity.x * dt;
                        second.transform.translation.y -= second.velocity.y * dt;
                                        }
                }
            }
        };

        void system_updateables_position(float dt)
        {
            auto updateables = world.template view<SquareComponent, RigidBodyComponent>();
            updateables.for_each([&dt](SquareComponent &s, RigidBodyComponent &rb)
                                 {
                                     float xvel = rb.velocity.x;
                                     float yvel = rb.velocity.y;

                                     
                                     rb.transform.translation.x += (xvel * dt);
                                     rb.transform.translation.y += (yvel * dt);
                                     s.ang = rb.transform.rotation.x;
                                     s.rect.x = rb.transform.translation.x;
                                     s.rect.y = rb.transform.translation.y; });
        }

        void system_updateables_particles(float dt)
        {
            auto updateables = world.template view<ParticleComponent>();
            updateables.for_each([&dt](ParticleComponent &pc)
                                 {
                                   
                                     float xvel = pc.rigidbody.velocity.x;
                                     float yvel = pc.rigidbody.velocity.y;
                                    
                                     pc.rigidbody.transform.translation.x += (xvel * dt);
                                     pc.rigidbody.transform.translation.y += (yvel * dt); });
        }

    public:
        PhysicsManager(snek::world<WorldPolicy> &w) : world(w) {};

        void update(float dt)
        {
            if constexpr (WorldPolicy::template is_valid_component_set<SquareComponent, RigidBodyComponent>())
            {
                system_updateables_position(dt);
            }
            if constexpr (WorldPolicy::template is_valid_component<RigidBodyComponent>())
            {
                system_updateables_collider(dt);
            }
            if constexpr (WorldPolicy::template is_valid_component<ParticleComponent>())
            {
                system_updateables_particles(dt);
            }
        }
    };
}