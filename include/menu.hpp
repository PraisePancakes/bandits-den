#pragma once
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "state_manager.hpp"
#include "systems/drawables/system_ui.hpp"
#include "systems/drawables/system_drawables.hpp"
#include "algorithm"
#include <string>
#include <cmath>

namespace bden::state
{
    class state_menu final : public AppStateType
    {
        using world_policy = bden::config::menu_config::menu_configuration_policy;
        snek::world<world_policy> world;
        bden::systems::DrawableManager<world_policy> render_system; // for menu particles
        bden::systems::UIManager<world_policy> ui_system;           // for menu ui
        world_policy::entity_type title;

        void init_menu_title()
        {
            auto e = world.spawn(world_policy::tag_type::TAG_TITLE);
            title = e;
            Transform loc{};
            loc.translation.x = GetScreenWidth() / 2;
            loc.translation.y = GetScreenHeight() / 2;
            world.bind<TextComponent>(e, loc, "bandits-den", RED, 40);
        };

        void init_menu_play_text()
        {
            auto title_text = world.get<TextComponent>(title);
        };

        void init_menu_entities()
        {
            init_menu_title();
        };

    public:
        state_menu(AppStateManagerType *ctx) : State(ctx), render_system(world), ui_system(world)
        {
            init_menu_entities();
        };
        void on_update(float dt) override
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                this->get_context()->set_state(AppStateManagerType::states_type::STATE_GAME);
            }
        };

        void on_render() override
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            render_system.render();
            ui_system.render();
            EndDrawing();
        };

        void on_exit() override {};

        ~state_menu() {};
    };
};
