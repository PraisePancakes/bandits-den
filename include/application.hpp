#pragma once
#include "../types.hpp"
#include <iostream>
#include "raylib.h"
#include "game.hpp"
#include "menu.hpp"
#include "app_observer.hpp"
#include <list>

namespace bden::applicationlayer
{
    class application final : private application_subject
    {
    public:
        int win_width;
        int win_height;

    private:
        std::string win_title;
        unsigned int config_flags;
        bden::gamelayer::state_game *game;
        bden::gamelayer::state_menu *menu;
        std::list<application_observer *> states;
        application_observer *current_state{nullptr};

        void attach(application_observer *observer) override
        {
            states.push_back(observer);
        }

        void detach(application_observer *observer) override
        {
            states.remove(observer);
        };

        void update(float dt)
        {
            current_state->on_update(dt);
        };

        void render()
        {
            current_state->on_render();
        }

    public:
        application(int w, int h, const std::string &t, unsigned int flags) : win_width(w), win_height(h), win_title(t), config_flags(flags)
        {
            SetConfigFlags(flags);
            InitWindow(w, h, t.c_str());
            SetTargetFPS(60);

            game = new bden::gamelayer::state_game();
            menu = new bden::gamelayer::state_menu();
            current_state = game;
            attach(game);
            attach(menu);
        };

        void run()
        {

            while (!WindowShouldClose()) // Detect window close button or ESC key
            {
                float dt = GetFrameTime();
                win_width = GetScreenWidth();
                win_height = GetScreenHeight();
                notify();
                update(dt);
                render();
            }
        };

        void notify() override
        {
            for (auto &o : states)
            {
                o->update_app_listener(win_width, win_height);
            }
        };
        ~application() = default;
    };
};