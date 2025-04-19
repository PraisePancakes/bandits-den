#pragma once
#include "types.hpp"
#include <iostream>
#include "raylib.h"
#include "game.hpp"

namespace bden::layer
{
    class application
    {
        int win_width;
        int win_height;
        std::string win_title;
        unsigned int config_flags;
        bden::layer::game game;

    public:
        application(int w, int h, const std::string &t, unsigned int flags) : win_width(w), win_height(h), win_title(t), config_flags(flags)
        {
            SetConfigFlags(flags);
            InitWindow(w, h, t.c_str());
            SetTargetFPS(60);
        };
        void run()
        {
            while (!WindowShouldClose()) // Detect window close button or ESC key
            {
                game.loop();
            }
        };
        ~application() {};
    };
};