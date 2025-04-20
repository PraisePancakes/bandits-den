#pragma once
#include <iostream>
#include "raylib.h"
#include "../vendor/SnakeECS/snakeecs/snakeecs.hpp"
#include "components/square.hpp"
#include "components/transform.hpp"
#include "app_observer.hpp"
#include <cmath>
#include "rlgl.h"

namespace bden::gamelayer
{
    using namespace components;
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
        WorldType::entity_type spawn_player(float w, float h, float x, float y, Color c)
        {
            auto p = world.spawn();
            SquareComponent square{w, h, x, y, c, 0.0};
            world.bind<SquareComponent>(p, square);
            world.bind<TransformComponent>(p, Vector2(x, y), Vector2(0, 0), 0.0);
            return p;
        };
#define PLAYER_SPEED 5
        void system_input_player_keys()
        {
            // update players velocity/movement
            auto &sc = world.get<SquareComponent>(player);
            auto &vel = world.get<TransformComponent>(player).vel;
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
        }

        void system_input_player_mouse()
        {
            auto &tc = world.get<TransformComponent>(player);
            auto &ang = tc.ang;

            rlPushMatrix();
            float rot = ((int)(++ang) % 360);
            rlRotatef(rot, tc.pos.x, tc.pos.y, 0);
            rlPopMatrix();
        };

        void system_input()
        {
            system_input_player_keys();
            system_input_player_mouse();
        };

        void system_updateables()
        {
            auto updateables = world.view<SquareComponent, TransformComponent>();
            updateables.for_each([](SquareComponent &s, TransformComponent &t)
                                 {
                t.pos.x += t.vel.x;
                t.pos.y += t.vel.y;
                s.ang = t.ang;
                s.rect.x = t.pos.x;
                s.rect.y = t.pos.y; });
        };

        void system_drawables()
        {
            auto drawables = world.view<SquareComponent>();
            drawables.for_each([this](SquareComponent &c)
                               {
                                   // DrawRectangle(c.x, c.y, c.width, c.height, c.color);
                                   DrawRectanglePro({c.rect.x, c.rect.y, c.rect.width, c.rect.height}, {c.rect.width / 2, c.rect.height / 2}, c.ang, c.color); });
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
