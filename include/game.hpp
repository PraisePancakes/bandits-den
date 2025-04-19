#pragma once
#include <iostream>
#include "raylib.h"
#include "../vendor/SnakeECS/snakeecs/snakeecs.hpp"
#include "components/square.hpp"

namespace bden::layer
{

    namespace world
    {
        using component_list = snek::component_list<components::SquareComponent>;
        snek::world_policy<u64, component_list, std::allocator<u64>> configuration_policy;
    }

    class game
    {
    public:
        game() {};
        void render()
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            EndDrawing();
        };
        void loop()
        {
            render();
        };
        ~game() {};
    };
};
