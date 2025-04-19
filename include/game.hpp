#pragma once
#include <iostream>
#include "raylib.h"
#include "../vendor/SnakeECS/snakeecs/snakeecs.hpp"
#include "components/square.hpp"
#include "app_observer.hpp"

namespace bden::gamelayer
{

    namespace world
    {
        using component_list = snek::component_list<components::SquareComponent>;
        using configuration_policy = snek::world_policy<u64, component_list, std::allocator<u64>>;
    }

    class game final : public applicationlayer::application_observer
    {
        int screen_width;
        int screen_height;

        using WorldType = snek::world<world::configuration_policy>;
        WorldType world;
        WorldType::entity_type player;
        WorldType::entity_type spawn_player(int w, int h, int x, int y, Color c)
        {
            auto p = world.spawn();
            components::SquareComponent square{w, h, x, y, c};
            world.bind<components::SquareComponent>(p, square);
            return p;
        };

        void system_drawables()
        {
            auto drawables = world.view<components::SquareComponent>();
            drawables.for_each([](components::SquareComponent &c)
                               { DrawRectangle(c.x, c.y, c.width, c.height, c.color); });
        };

        void update_application_metadata(int w, int h) override
        {
            std::cout << "updated app data from listener";
            screen_width = w;
            screen_height = h;
        };

    public:
        game() : player(spawn_player(100, 100, 500, 500, RED)) {

                 };

        void render()
        {
            BeginDrawing();
            ClearBackground(BLACK);
            system_drawables();
            EndDrawing();
        };
        void loop()
        {
            render();
        };
        ~game() {};
    };
};
