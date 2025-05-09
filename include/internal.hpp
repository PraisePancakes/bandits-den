#pragma once
#include "../vendor/SnakeECS/snakeecs/snakeecs.hpp"
#include "components/square.hpp"
#include "components/rigidbody.hpp"
#include "components/circle.hpp"
#include "components/health.hpp"
#include "components/agro.hpp"
#include "components/weapon.hpp"
#include "../types.hpp"

namespace bden::gamelayer::internal
{
    using component_list = snek::component_list<components::SquareComponent,
                                                components::RigidBodyComponent,
                                                components::HealthComponent,
                                                components::CircleComponent,
                                                components::AggroComponent,
                                                components::WeaponComponent>;

    enum class TagEnum : u64
    {
        TAG_PLAYER,
        TAG_ENEMIES,
        TAG_FRIENDLIES
    } Tags;

    using configuration_policy = snek::world_policy<u64, component_list, std::allocator<u64>>;

}