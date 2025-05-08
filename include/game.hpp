#pragma once
#include <iostream>
#include "raylib.h"
#include "internal.hpp"
#include "raymath.h"
#include "app_observer.hpp"
#include "systems/system_camera.hpp"
#include "systems/system_physics.hpp"
#include "systems/system_input.hpp"

#include "algorithm"
#include <string>
#include <cmath>
#include "rlgl.h"

namespace bden::gamelayer
{
    using namespace components;
    using namespace internal;

    // TODO create glow effect for entites

    class game final : public applicationlayer::application_observer
    {

        using WorldType = snek::world<configuration_policy>;
        WorldType world;
        WorldType::entity_type player;
        WorldType::entity_type test;
        std::vector<WorldType::entity_type> to_delete;
        systems::CameraManager<WorldType::world_policy> camera_system;
        systems::PhysicsManager<WorldType::world_policy> physics_system;
        systems::InputManager<WorldType::world_policy> input_system;
        int screen_width = 0;
        int screen_height = 0;

        void system_updateables_delete_entities()
        {
            for (auto &e : to_delete)
            {
                world.kill(e);
            }
            to_delete.clear();
        }

        void system_updateables_health()
        {
            static const auto health_bottom_space = 50;
            auto updateables = world.view<SquareComponent, RigidBodyComponent, HealthComponent>();
            updateables.for_each([this](WorldType::entity_type e, const SquareComponent &sqc, const RigidBodyComponent &rbc, HealthComponent &hc)
                                 {
                                    hc.health_bar.x = rbc.transform.translation.x - (sqc.rect.width / 2);
                                    hc.health_bar.y = rbc.transform.translation.y - (sqc.rect.height / 2) - health_bottom_space;
                                    hc.health_bar.width -= (int)hc.health_bar.width % (int)hc.hit_points ;
                                   
                if(hc.hit_points <= 1) {
                 
                       this->to_delete.push_back(e);
                }; });
        }

        WorldType::entity_type spawn_player(float w, float h, float x, float y, Color player_color, Color glow_color)
        {
            auto p = world.spawn((WorldType::entity_type)TagEnum::TAG_PLAYER);
            SquareComponent square{w, h, x, y, player_color, 0.0};
            world.bind<SquareComponent>(p, square);
            Transform player_transform{};
            player_transform.translation.x = x;
            player_transform.translation.y = y;

            const auto c = world.bind<CircleComponent>(p, square.rect.width, glow_color);
            Vector2 health_bar_pos(player_transform.translation.x, player_transform.translation.y - c.radius);
            Vector2 health_bar_size(w, 15.f);
            Rectangle health_bar_rect(health_bar_pos.x, health_bar_pos.y, health_bar_size.x, health_bar_size.y);
            world.bind<HealthComponent>(p, 100, health_bar_rect);
            world.bind<RigidBodyComponent>(p, player_transform, Vector2(0, 0), c.radius);

            return p;
        };

        WorldType::entity_type spawn_test(float w, float h, float x, float y, Color test_color, Color glow_color)
        {
            auto test = world.spawn((WorldType::entity_type)TagEnum::TAG_ENEMIES);
            SquareComponent square{w, h, x, y, test_color, 0.0};
            world.bind<SquareComponent>(test, square);
            Transform test_transform{};
            test_transform.translation.x = x;
            test_transform.translation.y = y;
            world.bind<AggroComponent>(test, w * 2, false);

            auto c = world.bind<CircleComponent>(test, square.rect.width, glow_color);
            world.bind<RigidBodyComponent>(test, test_transform, Vector2(0, 0), c.radius);
            return test;
        };

        void update_app_listener(int w, int h) override
        {
            screen_width = w;
            screen_height = h;
            camera_system.update_app_listener(w, h);
        }
#define PLAYER_SPEED 250
        void system_updateables_aggro()
        {
            auto updateables = world.view<RigidBodyComponent, AggroComponent>();
            auto &prb = world.get_ref<RigidBodyComponent>(player);

            updateables.for_each([&prb, this](RigidBodyComponent &rb, AggroComponent &ac)
                                 {
                                
                                        float xsquared = (rb.transform.translation.x - prb.transform.translation.x) * (rb.transform.translation.x - prb.transform.translation.x);
                                        float ysquared = (rb.transform.translation.y - prb.transform.translation.y) * (rb.transform.translation.y - prb.transform.translation.y);
                                        float dist = std::sqrt(xsquared + ysquared);
                                        bool aggroed = dist < prb.collision_radius + ac.aggro_radius;
                                        rb.velocity.x = 0;
                                        rb.velocity.y = 0;
                                    if(aggroed) {
                                        auto player_pos = Vector2(prb.transform.translation.x, prb.transform.translation.y);
                                        float x =  player_pos.x - rb.transform.translation.x ;
                                        float y = player_pos.y - rb.transform.translation.y ;
                                        auto &ang = rb.transform.rotation.x;
                                        float rad = atan2(x, y);
                                        float deg = (rad * 180.0) / PI;
                                       
                                        ang = -deg;
                                        rlPushMatrix();
                                        rlRotatef(ang, x, y, 0);
                                        rlPopMatrix();

                                        //move
                                           rb.velocity.x += (x > 0 ? 1 : -1);
                                           rb.velocity.y += (y > 0 ? 1 : -1);
                                           auto mag = std::sqrt(rb.velocity.x * rb.velocity.x + rb.velocity.y  * rb.velocity.y );
    
                                           if (mag != 0.0)
                                           {
                                            rb.velocity.x = (rb.velocity.x / mag);
                                            rb.velocity.y = (rb.velocity.y / mag);
                                           }
                                        rb.velocity.x *= PLAYER_SPEED / 2;
                                        rb.velocity.y *= PLAYER_SPEED / 2;
                                     
                                      
                                    } });
        };

        void system_updateables(float dt)
        {
            if (!world.contains(player))
                return;

            physics_system.update(dt, player);
            camera_system.update(dt, player);
            // TO DO separate player speed and camera system from input system
            input_system.update(player, camera_system, PLAYER_SPEED);

            system_updateables_health();
            system_updateables_aggro();
            system_updateables_delete_entities();
        };

        void system_drawables_shapes()
        {
            auto drawable_entities = world.view<SquareComponent, CircleComponent>();
            drawable_entities.for_each([this](WorldType::entity_type e, SquareComponent &sc, CircleComponent &cc)
                                       {
                                      
                                   // DrawRectangle(c.x, c.y, c.width, c.height, c.color);
                                   if(!world.contains(e)) return;
                                   DrawCircleGradient(sc.rect.x, sc.rect.y, cc.radius, WHITE, cc.color);
                                   DrawRectanglePro({sc.rect.x, sc.rect.y, sc.rect.width, sc.rect.height}, {sc.rect.width / 2, sc.rect.height / 2}, sc.ang, sc.color); });
        };

        Color get_health_color(float hp)
        {
            if (hp >= 50 && hp < 75)
                return YELLOW;
            else if (hp < 50 && hp >= 0)
                return RED;
            return GREEN;
        };

        void system_drawables_health()
        {
            auto drawable_entities = world.view<HealthComponent>();
            drawable_entities.for_each([this](WorldType::entity_type e, const HealthComponent &hc)
                                       {
                                        if (!world.contains(e)) return;
                                        DrawRectangleLines(hc.health_bar.x, hc.health_bar.y, hc.health_bar.width + 1, hc.health_bar.height + 1, BEIGE);
                                        DrawText(TextFormat("%d", hc.hit_points), hc.health_bar.x + hc.health_bar.width + 5, hc.health_bar.y, 20, WHITE);
                                        DrawRectangleRec(hc.health_bar, get_health_color(hc.hit_points)); });
        };

        void system_drawables()
        {
            system_drawables_shapes();
            system_drawables_health();
        };

    public:
        game() : player(spawn_player(100, 100, 500, 500, RED, {253, 76, 167, 47})),
                 test(spawn_test(100, 100, 200, 200, BLUE, {253, 76, 167, 47})),
                 camera_system(world),
                 physics_system(world),
                 input_system(world) {

                 };

        void update(float dt)
        {
            system_updateables(dt);
        };

        void render()
        {

            BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(camera_system.get_camera());
            system_drawables();
            EndMode2D();
            EndDrawing();
        };
        void loop()
        {

            float dt = GetFrameTime();
            update(dt);

            render();
        };
        ~game() = default;
    };
};
