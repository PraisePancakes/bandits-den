#pragma once
#include <iostream>
#include "raylib.h"
#include "../../types.hpp"
#include "../../vendor/SnakeECS/snakeecs/snakeecs.hpp"
#include "../components/agro.hpp"
#include "../components/circle.hpp"
#include "../components/health.hpp"
#include "../components/rigidbody.hpp"
#include "../components/square.hpp"
#include "../components/triangle.hpp"
#include "../components/weapon.hpp"
#include "../components/button.hpp"

namespace bden::config::GameConfig
{
    using game_component_list = snek::component_list<components::SquareComponent,
                                                     components::RigidBodyComponent,
                                                     components::HealthComponent,
                                                     components::CircleComponent,
                                                     components::AggroComponent,
                                                     components::WeaponComponent,
                                                     components::ButtonComponent>;

    enum class TagEnum : u64
    {
        TAG_PLAYER,
        TAG_ENEMIES,
        TAG_FRIENDLIES
    };

    using game_configuration_policy = snek::world_policy<u64, game_component_list, TagEnum>;

}