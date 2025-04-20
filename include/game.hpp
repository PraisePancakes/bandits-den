#pragma once
#include <iostream>
#include "raylib.h"
#include "../vendor/SnakeECS/snakeecs/snakeecs.hpp"
#include "components/square.hpp"
#include "components/transform.hpp"
#include "app_observer.hpp"
#include <cmath>

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
#define PLAYER_SPEED 5
        void system_input()
        {
            auto &sc = world.get<components::SquareComponent>(player);
            auto &vel = world.get<components::TransformComponent>(player).vel;
            auto &pos = world.get<components::TransformComponent>(player).pos;
            vel.x = 0;
            vel.y = 0;
            // dir
            if (IsKeyDown(KEY_W))
            {
                vel.y -= 1;
            }
            if (IsKeyDown(KEY_A))
            {
                vel.x -= 1;
            }
            if (IsKeyDown(KEY_S))
            {
                vel.y += 1;
            }
            if (IsKeyDown(KEY_D))
            {
                vel.x += 1;
            }

            // normalize velocity vector
            auto mag = std::sqrt(vel.x * vel.x + vel.y * vel.y);

            if (mag != 0.0)
            {
                vel.x = (vel.x / mag);
                vel.y = (vel.y / mag);
            }

            // apply speed to the velocity
            vel.x *= PLAYER_SPEED;
            vel.y *= PLAYER_SPEED;

            // update position
            pos.x += vel.x;
            pos.y += vel.y;

            // update square
            sc.x = pos.x;
            sc.y = pos.y;
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
