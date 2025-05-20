#pragma once
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "../../components/rigidbody.hpp"
#include "../../components/weapon.hpp"
#include "../../utils.hpp"

namespace bden::systems
{
    using namespace components;

    template <typename WorldPolicy>
    class WeaponManager final
    {
        snek::world<WorldPolicy> &world;

        void system_updateables_weapon()
        {
            auto updateables = world.template view<RigidBodyComponent, WeaponComponent>();

            updateables.for_each([](RigidBodyComponent &rb, WeaponComponent &wc)
                                 {
                                     Vector2 center = {rb.transform.translation.x, rb.transform.translation.y};
                                     float theta = rb.transform.rotation.x * DEG2RAD;
                                     Vector2 direction = {std::cos(theta), std::sin(theta)};
                                     wc.weapon_ray = {center.x + direction.x * wc.radius, center.y + direction.y * wc.radius}; });
        };

    public:
        WeaponManager(snek::world<WorldPolicy> &w) : world(w) {};

        void update(float dt)
        {

            if constexpr (WorldPolicy::template is_valid_component_set<RigidBodyComponent, WeaponComponent>())
            {
                system_updateables_weapon();
            }
        }
    };
}