#pragma once
#include <iostream>
#include "raylib.h"
#include "../vendor/SnakeECS/snakeecs/snakeecs.hpp"
#include "components/square.hpp"
#include "components/rigidbody.hpp"
#include "components/circle.hpp"

#include "app_observer.hpp"
#include <cmath>
#include "rlgl.h"

namespace bden::gamelayer
{
    using namespace components;
    namespace world
    {
        using component_list = snek::component_list<components::SquareComponent,
                                                    components::RigidBodyComponent,
                                                    components::CircleComponent>;

        using configuration_policy = snek::world_policy<u64, component_list, std::allocator<u64>>;
    }
    // TODO create glow effect for entites

    class game final : public applicationlayer::application_observer
    {
        int screen_width = 0;
        int screen_height = 0;

        using WorldType = snek::world<world::configuration_policy>;
        WorldType world;
        WorldType::entity_type player;
        WorldType::entity_type test;
        Camera2D camera{};

        void system_updateables_camera()
        {
            const auto &ptc = world.get<RigidBodyComponent>(player).transform;
            camera.target = {ptc.translation.x, ptc.translation.y};
            camera.offset = {screen_width / 2.0f, screen_height / 2.0f};
            camera.rotation = 0.0f;
            camera.zoom = 1.0f;
        };

        WorldType::entity_type spawn_player(float w, float h, float x, float y, Color player_color, Color glow_color)
        {
            auto p = world.spawn();
            SquareComponent square{w, h, x, y, player_color, 0.0};
            world.bind<SquareComponent>(p, square);
            Transform player_transform{};
            player_transform.translation.x = x;
            player_transform.translation.y = y;

            const auto c = world.bind<CircleComponent>(p, square.rect.width, glow_color);
            world.bind<RigidBodyComponent>(p, player_transform, Vector2(0, 0), c.radius);

            return p;
        };

        WorldType::entity_type spawn_test(float w, float h, float x, float y, Color test_color, Color glow_color)
        {
            auto test = world.spawn();
            SquareComponent square{w, h, x, y, test_color, 0.0};
            world.bind<SquareComponent>(test, square);
            Transform test_transform{};
            test_transform.translation.x = x;
            test_transform.translation.y = y;

            auto c = world.bind<CircleComponent>(test, square.rect.width, glow_color);
            world.bind<RigidBodyComponent>(test, test_transform, Vector2(0, 0), c.radius);
            return test;
        };
#define PLAYER_SPEED 5
        void system_updateables_input_player_keys()
        {
            // update players velocity/movement
            auto &vel = world.get<RigidBodyComponent>(player).velocity;
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
            auto &tc = world.get<RigidBodyComponent>(player).transform;
            auto &sc = world.get<SquareComponent>(player);
            auto &ang = tc.rotation.x;
            const float x = tc.translation.x;
            const float y = tc.translation.y;
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
            auto updateables = world.view<SquareComponent, RigidBodyComponent>();
            updateables.for_each([](SquareComponent &s, RigidBodyComponent &t)
                                 {
                t.transform.translation.x += t.velocity.x;
                t.transform.translation.y += t.velocity.y;
                s.ang = t.transform.rotation.x;
                s.rect.x = t.transform.translation.x;
                s.rect.y = t.transform.translation.y; });
        }

        void system_updateables_collider()
        {
            auto &prb = world.get<RigidBodyComponent>(player);
            auto collideables = world.view<RigidBodyComponent>();
            collideables.for_each([this, &prb](u64 id, RigidBodyComponent &rb)
                                  {
                                    if(id != this->player) {
                                        auto r = rb.collision_radius;
                                        float xsquared = (rb.transform.translation.x - prb.transform.translation.x) * (rb.transform.translation.x - prb.transform.translation.x);
                                        float ysquared = (rb.transform.translation.y - prb.transform.translation.y) * (rb.transform.translation.y - prb.transform.translation.y);
                                        float dist = std::sqrt(xsquared + ysquared);
                                        bool collided = dist < prb.collision_radius + r;
                                        if(collided) {
                                            prb.transform.translation.x += -prb.velocity.x;
                                            prb.transform.translation.y += -prb.velocity.y;
                                        }
                                    } });
        };

        void system_updateables()
        {
            system_updateables_input();
            system_updateables_position();
            system_updateables_collider();
            system_updateables_camera();
        };

        void system_drawables()
        {
            auto drawable_entities = world.view<SquareComponent, CircleComponent>();
            drawable_entities.for_each([this](SquareComponent &c, CircleComponent &cc)
                                       {
                                   // DrawRectangle(c.x, c.y, c.width, c.height, c.color);
                                   DrawCircleGradient(c.rect.x, c.rect.y, cc.radius, WHITE, cc.color);
                                   DrawRectanglePro({c.rect.x, c.rect.y, c.rect.width, c.rect.height}, {c.rect.width / 2, c.rect.height / 2}, c.ang, c.color); });
        };

        void update_app_listener(int w, int h) override
        {
            screen_width = w;
            screen_height = h;
        };

    public:
        game() : player(spawn_player(100, 100, 500, 500, RED, {253, 76, 167, 47})), test(spawn_test(100, 100, 200, 200, BLUE, {253, 76, 167, 47})) {

                 };

        void update()
        {
            system_updateables();
        };

        void render()
        {
            BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(camera);
            system_drawables();
            EndMode2D();
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
