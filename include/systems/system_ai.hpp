#pragma once
#include "../internal.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

namespace bden::gamelayer::systems
{
    using namespace internal;
    using namespace components;

    template <typename WorldPolicy>
    class AiManager final
    {
        snek::world<WorldPolicy> &world;

    public:
        AiManager(snek::world<WorldPolicy> &w) : world(w) {};

        void system_updateables_aggro(typename WorldPolicy::entity_type player, float PLAYER_SPEED)
        {
            auto updateables = world.template view<RigidBodyComponent, AggroComponent>();
            auto &prb = world.template get_ref<RigidBodyComponent>(player);

            updateables.for_each([&prb, this, &PLAYER_SPEED](RigidBodyComponent &rb, AggroComponent &ac)
                                 {
                                
                                        float xsquared = (rb.transform.translation.x - prb.transform.translation.x) * (rb.transform.translation.x - prb.transform.translation.x);
                                        float ysquared = (rb.transform.translation.y - prb.transform.translation.y) * (rb.transform.translation.y - prb.transform.translation.y);
                                        float dist = std::sqrt(xsquared + ysquared);
                                        bool aggroed = dist < prb.collision_radius + ac.aggro_radius;
                                        rb.velocity.x = 0;
                                        rb.velocity.y = 0;
                                    if(aggroed) {
                                        auto player_pos = Vector2(prb.transform.translation.x, prb.transform.translation.y);
                                        float x =  player_pos.x - rb.transform.translation.x ;
                                        float y = player_pos.y - rb.transform.translation.y ;
                                        auto &ang = rb.transform.rotation.x;
                                        float rad = atan2(x, y);
                                        float deg = (rad * 180.0) / PI;
                                       
                                        ang = -deg;
                                        rlPushMatrix();
                                        rlRotatef(ang, x, y, 0);
                                        rlPopMatrix();

                                        //move
                                           rb.velocity.x += (x > 0 ? 1 : -1);
                                           rb.velocity.y += (y > 0 ? 1 : -1);
                                           auto mag = std::sqrt(rb.velocity.x * rb.velocity.x + rb.velocity.y  * rb.velocity.y );
    
                                           if (mag != 0.0)
                                           {
                                            rb.velocity.x = (rb.velocity.x / mag);
                                            rb.velocity.y = (rb.velocity.y / mag);
                                           }
                                        rb.velocity.x *= PLAYER_SPEED / 2;
                                        rb.velocity.y *= PLAYER_SPEED / 2;
                                     
                                      
                                    } });
        };

        void update(float dt, WorldPolicy::entity_type player, float PLAYER_SPEED)
        {
            system_updateables_aggro(player, PLAYER_SPEED);
        }
    };
}