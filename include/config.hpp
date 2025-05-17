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
#include "components/particle.hpp"
#include "../vendor/SnakeECS/snakeecs/snakeecs.hpp"

namespace bden::config
{
    namespace app_config
    {
        constexpr static int VIRTUAL_WIDTH{1280};
        constexpr static int VIRTUAL_HEIGHT{720};
        constexpr static int WINDOW_FLAGS{FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI};
        const static std::string WINDOW_TITLE("bandits-den");
    }

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

        using game_configuration_policy = snek::world_policy<u64, game_component_list, std::allocator<u64>, TagEnum>;
    }

    namespace menu_config
    {
        using menu_component_list = snek::component_list<components::ButtonComponent, components::TextComponent, components::ParticleComponent>;
        enum class TagEnum : u64
        {
            TAG_TITLE,
            TAG_PLAY,
            TAG_QUIT,
            TAG_PARTICLE
        } Tags;
        constexpr static int PARTICLE_COUNT{500};
        constexpr static float PARTICLE_SPEED{250.f};
        constexpr static Color PARTICLE_COLOR{BLACK};
        using menu_configuration_policy = snek::world_policy<u64, menu_component_list, std::allocator<u64>, TagEnum>;
    }

    namespace weapons
    {
        namespace spear
        {
            constexpr static float radius{1.5f};
            constexpr static Color color{RED};
            constexpr static float damage{5.f};
            constexpr static float speed{2.f};
        }
    }

    namespace player
    {
        constexpr static float PLAYER_SPEED{250.f};
        constexpr static Color PLAYER_COLOR{BLUE};
        constexpr static Color PLAYER_GLOW{253, 243, 214, 100};
    } // player
    namespace enemies
    {
        constexpr static float ENEMY_SPEED{150.f};
        constexpr static Color ENEMY_GLOW{253, 243, 214, 100};
        constexpr static Color ENEMY_COLOR{RED};
    } // enemies

    namespace ui
    {
        namespace weapon_animations
        {
            constexpr static float START_ANGLE{0.f};
            constexpr static float END_ANGLE{360.f};
            constexpr static int NSEGS{10};

        } // weapons
    } // ui

};