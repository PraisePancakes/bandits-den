#pragma once
#include "../types.hpp"
#include <iostream>
#include "raylib.h"
#include "game.hpp"
#include "menu.hpp"
#include "app_observer.hpp"
#include "state_manager.hpp"
#include <list>

namespace bden::applicationlayer
{

    class application final : public application_subject
    {
    public:
        int win_width;
        int win_height;

    private:
        std::string win_title;
        unsigned int config_flags;
        bden::state::state_game *game;
        bden::state::state_menu *menu;
        std::list<application_observer *> observers;

        AppStateManagerType state_manager;

        void attach(application_observer *observer) override
        {
            observers.push_back(observer);
        }

        void detach(application_observer *observer) override
        {
            observers.remove(observer);
        };

        void notify() override
        {
            for (auto &o : observers)
            {
                o->update_app_listener(win_width, win_height);
            }
        };

    public:
        application(int w, int h, const std::string &t, unsigned int flags) : win_width(w), win_height(h), win_title(t), config_flags(flags)
        {
            SetConfigFlags(flags);
            InitWindow(w, h, t.c_str());
            SetTargetFPS(60);

            game = new bden::state::state_game(&state_manager);
            menu = new bden::state::state_menu(&state_manager);
            state_manager.insert_state(AppStateManagerType::states_type::STATE_MENU, menu);
            state_manager.insert_state(AppStateManagerType::states_type::STATE_GAME, game);

            attach(game);
            attach(menu);
            state_manager.set_state(AppStateManagerType::states_type::STATE_MENU);
        };

        void run()
        {

            while (!WindowShouldClose()) // Detect window close button or ESC key
            {
                float dt = GetFrameTime();
                win_width = GetScreenWidth();
                win_height = GetScreenHeight();
                notify();
                state_manager.get_current_state()->on_update(dt);
                state_manager.get_current_state()->on_render();
            }
        };

        ~application() = default;
    };
};