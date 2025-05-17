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
    class AiManager final
    {
        snek::world<WorldPolicy> &world;

        void system_updateables_aggro(typename WorldPolicy::entity_type player)
        {
            auto updateables = world.template view<RigidBodyComponent, AggroComponent>();
            auto &prb = world.template get_ref<RigidBodyComponent>(player);

            updateables.for_each([&prb, this](RigidBodyComponent &rb, AggroComponent &ac)
                                 {
                                        rb.velocity.x = 0;
                                        rb.velocity.y = 0;
                                    if(utils::collided(rb, prb, ac.aggro_radius)) {
                                        auto player_pos = Vector2(prb.transform.translation.x, prb.transform.translation.y);
                                        float x =  player_pos.x - rb.transform.translation.x ;
                                        float y = player_pos.y - rb.transform.translation.y ;
                                       
                                        float rad = atan2(y, x);
                                        float deg = (rad * 180.0) / PI;

                                        rb.transform.rotation.x = deg;
                                        rlPushMatrix();
                                        rlRotatef(rb.transform.rotation.x, x, y, 0);
                                        rlPopMatrix();

                                        rb.velocity.x = (x > 0 ? 1 : -1);
                                        rb.velocity.y = (y > 0 ? 1 : -1);
                                     
                                      
                                    } });
        };

    public:
        AiManager(snek::world<WorldPolicy> &w) : world(w) {};

        void update(float dt, WorldPolicy::entity_type player)
        {
            if constexpr (WorldPolicy::template is_valid_component_set<RigidBodyComponent, AggroComponent>())
            {
                system_updateables_aggro(player);
            }
        }
    };
}