#pragma once
#include "../types.hpp"
#include <iostream>
#include "raylib.h"
#include "game.hpp"
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
        bden::gamelayer::game *game;
        std::list<application_observer *> observers;

        void attach(application_observer *observer) override
        {
            observers.push_back(observer);
        }

        void detach(application_observer *observer) override
        {
            observers.remove(observer);
        };

    public:
        application(int w, int h, const std::string &t, unsigned int flags) : win_width(w), win_height(h), win_title(t), config_flags(flags)
        {
            SetConfigFlags(flags);
            InitWindow(w, h, t.c_str());
            SetTargetFPS(60);
            game = new bden::gamelayer::game();
            attach(game);
        };

        void run()
        {
            int prev_width = win_width;
            int prev_height = win_height;

            while (!WindowShouldClose()) // Detect window close button or ESC key
            {
                if (GetScreenWidth() != prev_width || GetScreenHeight() != prev_height)
                {
                    prev_width = GetScreenWidth();
                    prev_height = GetScreenHeight();
                    notify();
                }
                game->loop();
            }
        };

        void notify() override
        {
            for (auto &o : observers)
            {
                o->update_application_metadata(win_width, win_height);
            }
        };
        ~application() = default;
    };
};