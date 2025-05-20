#pragma once
#include <iostream>
#include "raylib.h"
#include "../../types.hpp"
#include "../../vendor/SnakeECS/snakeecs/snakeecs.hpp"
#include "../components/button.hpp"
#include "../components/text.hpp"
#include "../components/particle.hpp"

namespace bden::config::MenuConfig
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
    constexpr static Color PARTICLE_COLOR{RED};
    using menu_configuration_policy = snek::world_policy<u64, menu_component_list, std::allocator<u64>, TagEnum>;

}