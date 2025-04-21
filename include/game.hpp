#pragma once
#include <iostream>
#include "raylib.h"
#include "../vendor/SnakeECS/snakeecs/snakeecs.hpp"
#include "components/square.hpp"
#include "components/transform.hpp"
#include "components/circle.hpp"
#include "components/collider.hpp"
#include "app_observer.hpp"
#include <cmath>
#include "rlgl.h"

namespace bden::gamelayer
{
    using namespace components;
    namespace world
    {
        using component_list = snek::component_list<components::SquareComponent,
                                                    components::TransformComponent,
                                                    components::CircleComponent,
                                                    components::ColliderComponent>;

        using configuration_policy = snek::world_policy<u64, component_list, std::allocator<u64>>;
    }
    // TODO create glow effect for entites

    class game final : public applicationlayer::application_observer
    {
        int screen_width;
        int screen_height;

        using WorldType = snek::world<world::configuration_policy>;
        WorldType world;
        WorldType::entity_type player;
        WorldType::entity_type test;

        WorldType::entity_type spawn_player(float w, float h, float x, float y, Color player_color, Color glow_color)
        {
            auto p = world.spawn();
            SquareComponent square{w, h, x, y, player_color, 0.0};
            world.bind<SquareComponent>(p, square);
            world.bind<TransformComponent>(p, Vector2(x, y), Vector2(0, 0), 0.0);
            world.bind<CircleComponent>(p, square.rect.width, glow_color);
            world.bind<ColliderComponent>(p, square.rect.width);

            return p;
        };

        WorldType::entity_type spawn_test(float w, float h, float x, float y, Color test_color, Color glow_color)
        {
            auto p = world.spawn();
            SquareComponent square{w, h, x, y, test_color, 0.0};
            world.bind<SquareComponent>(p, square);
            world.bind<TransformComponent>(p, Vector2(x, y), Vector2(0, 0), 0.0);
            world.bind<CircleComponent>(p, square.rect.width, glow_color);
            world.bind<ColliderComponent>(p, square.rect.width);
            return p;
        };
#define PLAYER_SPEED 5
        void system_updateables_input_player_keys()
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

        void system_updateables_input_player_mouse()
        {
            auto &tc = world.get<TransformComponent>(player);
            auto &sc = world.get<SquareComponent>(player);
            auto &ang = tc.ang;
            const float x = tc.pos.x;
            const float y = tc.pos.y;
            const Vector2 mouse_pos = GetMousePosition();
            const Vector2 changed = {x - mouse_pos.x, y - mouse_pos.y};
            float rad = atan2(changed.x, changed.y);
            float deg = (rad * 180.0) / PI;

            ang = -deg;
            rlPushMatrix();
            rlRotatef(ang, x, y, 0);
            rlPopMatrix();
        };

        void system_updateables_input()
        {
            system_updateables_input_player_keys();
            system_updateables_input_player_mouse();
        };

        void system_updateables_position()
        {
            auto updateables = world.view<SquareComponent, TransformComponent>();
            updateables.for_each([](SquareComponent &s, TransformComponent &t)
                                 {
                t.pos.x += t.vel.x;
                t.pos.y += t.vel.y;
                s.ang = t.ang;
                s.rect.x = t.pos.x;
                s.rect.y = t.pos.y; });
        }

        void system_updateables_collider()
        {
            auto pcr = world.get<ColliderComponent>(player).radius;
            auto ptc = world.get<TransformComponent>(player);
            auto collideables = world.view<ColliderComponent, TransformComponent>();
            collideables.for_each([this, &ptc, &pcr](u64 id, ColliderComponent &c, TransformComponent &tc)
                                  {
                                    if(id != this->player) {
                                        auto r = c.radius;
                                        float xsquared = (tc.pos.x - ptc.pos.x) * (tc.pos.x - ptc.pos.x);
                                        float ysquared = (tc.pos.y - ptc.pos.y) * (tc.pos.y - ptc.pos.y);
                                        float dist = std::sqrt(xsquared + ysquared);
                                        bool collided = dist < pcr + r;

                                        if(collided) {
                                            std::cout << "collided" << std::endl;
                                            //determine from where collision occurs
                                        }
                                    } });
        };

        void system_updateables()
        {
            system_updateables_input();
            system_updateables_position();
            system_updateables_collider();
        };

        void system_drawables()
        {
            auto drawables = world.view<SquareComponent, CircleComponent>();
            drawables.for_each([this](SquareComponent &c, CircleComponent &cc)
                               {
                                   // DrawRectangle(c.x, c.y, c.width, c.height, c.color);
                                   
                                   DrawRectanglePro({c.rect.x, c.rect.y, c.rect.width, c.rect.height}, {c.rect.width / 2, c.rect.height / 2}, c.ang, c.color); 
                                   DrawCircle(c.rect.x, c.rect.y, cc.radius, cc.color); });
        };

        void update_app_listener(int w, int h) override
        {
            screen_width = w;
            screen_height = h;
        };

    public:
        game() : player(spawn_player(100, 100, 500, 500, RED, {253, 76, 167, 47})), test(spawn_test(100, 100, 500, 500, BLUE, {253, 76, 167, 47})) {

                 };

        void update()
        {
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
