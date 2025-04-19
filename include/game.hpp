#pragma once
#include <iostream>
#include "raylib.h"
#include "../vendor/SnakeECS/snakeecs/snakeecs.hpp"
#include "components/square.hpp"
#include "components/transform.hpp"
#include "app_observer.hpp"

namespace bden::gamelayer
{

    namespace world
    {
        using component_list = snek::component_list<components::SquareComponent, components::TransformComponent>;
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
            world.bind<components::TransformComponent>(p, Vector2(x, y), Vector2(0, 0));
            return p;
        };
#define PLAYER_SPEED 10
        void system_input()
        {
            auto &sc = world.get<components::SquareComponent>(player);
            auto &tc = world.get<components::TransformComponent>(player);

            if (IsKeyDown(KEY_W))
            {
            }
            if (IsKeyDown(KEY_A))
            {
            }
            if (IsKeyDown(KEY_S))
            {
            }
            if (IsKeyDown(KEY_D))
            {
            }
        };

        void system_updateables()
        {
            auto updateables = world.view<components::SquareComponent, components::TransformComponent>();
            updateables.for_each([](components::SquareComponent &s, components::TransformComponent &t)
                                 {
                s.x = t.pos.x;
                s.y = t.pos.y; });
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

        void update()
        {
            system_input();
            system_updateables();
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
            update();
            render();
        };
        ~game() {};
    };
};
