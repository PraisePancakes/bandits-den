#pragma once
#include "components/agro.hpp"
#include "components/circle.hpp"
#include "components/health.hpp"
#include "components/rigidbody.hpp"
#include "components/square.hpp"
#include "components/triangle.hpp"
#include "components/weapon.hpp"
#include "components/button.hpp"
#include "components/text.hpp"
#include "../vendor/SnakeECS/snakeecs/snakeecs.hpp"

namespace bden::config
{
    namespace game_config
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
        } Tags;

        using game_configuration_policy = snek::world_policy<u64, game_component_list, std::allocator<u64>>;
    }

    namespace menu_config
    {
        using menu_component_list = snek::component_list<components::ButtonComponent, components::TextComponent>;

        using menu_configuration_policy = snek::world_policy<u64, menu_component_list, std::allocator<u64>>;
    }

    namespace player
    {
        constexpr static float PLAYER_SPEED = 250.f;
    } // player
    namespace enemies
    {
        constexpr static float ENEMY_SPEED = player::PLAYER_SPEED / 2;
    } // enemies

    namespace ui
    {
        namespace weapons
        {
            constexpr static float START_ANGLE = 0.f;
            constexpr static float END_ANGLE = 360.f;
            constexpr static int NSEGS = 10;
        } // weapons
    } // ui

};