#pragma once
#include "../types.hpp"
#include <iostream>
#include "raylib.h"
#include "states/state_game.hpp"
#include "states/state_menu.hpp"
#include "states/state_quit.hpp"
#include "raymath.h"
#include "state_manager.hpp"
#include <list>

namespace bden::applicationlayer
{
    using namespace bden::config;
    class application final
    {

    private:
        std::string win_title;
        unsigned int config_flags;
        bden::state::state_game *game;
        bden::state::state_menu *menu;
        bden::state::state_quit *quit;

        bden::fsm::AppStateManagerType state_manager;
        RenderTexture2D render_target;

    public:
        application(int w, int h, const std::string &t, unsigned int flags)
        {

            SetConfigFlags(flags);
            InitWindow(w, h, t.c_str());

            render_target = LoadRenderTexture(w, h);
            SetTextureFilter(render_target.texture, TEXTURE_FILTER_BILINEAR);
            SetTargetFPS(60);
            SetExitKey(KEY_NULL);
            game = new bden::state::state_game(&state_manager, render_target);
            menu = new bden::state::state_menu(&state_manager, render_target);
            quit = new bden::state::state_quit(&state_manager); // will take reference to game to serialize its fields

            state_manager.insert_state(bden::fsm::AppStateManagerType::states_type::STATE_MENU, menu);
            state_manager.insert_state(bden::fsm::AppStateManagerType::states_type::STATE_GAME, game);
            state_manager.insert_state(bden::fsm::AppStateManagerType::states_type::STATE_QUIT, quit);
            state_manager.set_state(bden::fsm::AppStateManagerType::states_type::STATE_MENU);
        };

        void run()
        {
            while (state_manager.get_current_state() != quit)
            {
                float dt = GetFrameTime();
                state_manager.update_current_state(dt);
                state_manager.render_current_state();
            }
            state_manager.get_current_state()->on_exit();
        };
        ~application() {};
    };
};