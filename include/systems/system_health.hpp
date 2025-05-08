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
    class HealthManager final
    {
        snek::world<WorldPolicy> &world;
        std::vector<typename WorldPolicy::entity_type> &to_delete;

    public:
        HealthManager(snek::world<WorldPolicy> &w, std::vector<typename WorldPolicy::entity_type> &d) : world(w), to_delete(d) {};

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

        void update(float dt, WorldPolicy::entity_type player)
        {
            system_updateables_health_ui();
        }
    };
}