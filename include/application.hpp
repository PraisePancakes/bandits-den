#pragma once
#include "../types.hpp"
#include <iostream>
#include "raylib.h"
#include "game.hpp"
#include "menu.hpp"

#include "state_manager.hpp"
#include <list>

namespace bden::applicationlayer
{

    class application final
    {

    private:
        std::string win_title;
        unsigned int config_flags;
        bden::state::state_game *game;
        bden::state::state_menu *menu;

        AppStateManagerType state_manager;

    public:
        application(int w, int h, const std::string &t, unsigned int flags) : win_title(t), config_flags(flags)
        {
            SetConfigFlags(flags);
            InitWindow(w, h, t.c_str());
            SetTargetFPS(60);

            game = new bden::state::state_game(&state_manager);
            menu = new bden::state::state_menu(&state_manager);
            state_manager.insert_state(AppStateManagerType::states_type::STATE_MENU, menu);
            state_manager.insert_state(AppStateManagerType::states_type::STATE_GAME, game);

            state_manager.set_state(AppStateManagerType::states_type::STATE_MENU);
        };

        void run()
        {

            while (!WindowShouldClose()) // Detect window close button or ESC key
            {
                float dt = GetFrameTime();
                state_manager.get_current_state()->on_update(dt);
                state_manager.get_current_state()->on_render();
            }
        };

        ~application() = default;
    };
};