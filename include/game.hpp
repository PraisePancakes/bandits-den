#pragma once
#include <iostream>
#include "raylib.h"
#include "../vendor/SnakeECS/snakeecs/snakeecs.hpp"
#include "components/square.hpp"
#include "components/rigidbody.hpp"
#include "components/circle.hpp"
#include "components/health.hpp"
#include "raymath.h"
#include "app_observer.hpp"
#include "components/agro.hpp"
#include "algorithm"
#include <string>
#include <cmath>
#include "rlgl.h"

namespace bden::gamelayer
{
    using namespace components;
    namespace world
    {
        using component_list = snek::component_list<components::SquareComponent,
                                                    components::RigidBodyComponent,
                                                    components::HealthComponent,
                                                    components::CircleComponent,
                                                    components::AggroComponent>;

        enum class TagEnum : u64
        {
            TAG_PLAYER,
            TAG_ENEMIES,
        } Tags;

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

        void system_updateables_camera(float dt)
        {
            const float LERP_FACTOR = 5;
            const auto &ptc = world.get<RigidBodyComponent>(player).transform;
            auto lerpx = Lerp(camera.target.x, ptc.translation.x, dt * LERP_FACTOR);
            auto lerpy = Lerp(camera.target.y, ptc.translation.y, dt * LERP_FACTOR);

            camera.target = {lerpx, lerpy};
            camera.offset = {screen_width / 2.0f, screen_height / 2.0f};
            camera.rotation = 0.0f;
            camera.zoom = 1.0f;
        };
        void system_updateables_health()
        {
            static const auto health_bottom_space = 50;
            auto updateables = world.view<SquareComponent, RigidBodyComponent, HealthComponent>();
            updateables.for_each([this](WorldType::entity_type e, const SquareComponent &sqc, const RigidBodyComponent &rbc, HealthComponent &hc)
                                 {
                                    hc.health_bar.x = rbc.transform.translation.x - (sqc.rect.width / 2);
                                    hc.health_bar.y = rbc.transform.translation.y - (sqc.rect.height / 2) - health_bottom_space;
                                    hc.health_bar.width -= (int)hc.health_bar.width % (int)hc.hit_points ;
                                   
                if(hc.hit_points <= 0) {
                        this->world.kill(e);
                }; });
        }

        WorldType::entity_type spawn_player(float w, float h, float x, float y, Color player_color, Color glow_color)
        {
            auto p = world.spawn((WorldType::entity_type)world::TagEnum::TAG_PLAYER);
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
            auto test = world.spawn((WorldType::entity_type)world::TagEnum::TAG_ENEMIES);
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
#define PLAYER_SPEED 250
        void system_updateables_input_player_keys()
        {
            // update players velocity/movement
            auto &vel = world.get<RigidBodyComponent>(player).velocity;

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
            const Vector2 abs_mouse_pos = GetMousePosition();
            const Vector2 rel_screen_mouse = GetScreenToWorld2D(abs_mouse_pos, camera);
            const float x = rel_screen_mouse.x - tc.translation.x;
            const float y = rel_screen_mouse.y - tc.translation.y;

            float rad = atan2(x, y);
            float deg = (rad * 180.0) / PI;

            ang = -deg;
            rlPushMatrix();
            rlRotatef(ang, x, y, 0);
            rlPopMatrix();
        };
        void system_updateables_aggro()
        {
            auto updateables = world.view<RigidBodyComponent, AggroComponent>();
            auto prb = world.get<RigidBodyComponent>(player);

            updateables.for_each([&prb, this](RigidBodyComponent &rb, AggroComponent &ac)
                                 {
                                        float xsquared = (rb.transform.translation.x - prb.transform.translation.x) * (rb.transform.translation.x - prb.transform.translation.x);
                                        float ysquared = (rb.transform.translation.y - prb.transform.translation.y) * (rb.transform.translation.y - prb.transform.translation.y);
                                        float dist = std::sqrt(xsquared + ysquared);
                                        bool aggroed = dist < prb.collision_radius + ac.aggro_radius;
                                       
                                    if(aggroed) {
 
                                        /*
                                               auto r = rb.collision_radius;
                                        float xsquared = (rb.transform.translation.x - prb.transform.translation.x) * (rb.transform.translation.x - prb.transform.translation.x);
                                        float ysquared = (rb.transform.translation.y - prb.transform.translation.y) * (rb.transform.translation.y - prb.transform.translation.y);
                                        float dist = std::sqrt(xsquared + ysquared);
                                        bool collided = dist < prb.collision_radius + r;
                                        if(collided) {
                                            prb.transform.translation.x += (-prb.velocity.x * dt);
                                            prb.transform.translation.y += (-prb.velocity.y * dt);
                                            rb.transform.translation.x += (-rb.velocity.x * dt);
                                            rb.transform.translation.y += (-rb.velocity.y * dt);
                                            world.get<HealthComponent>(player).hit_points--;
                                        }
                                        */
                                        //rotate
                                      
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
                                        bool collided = dist < prb.collision_radius + rb.collision_radius;
                                        if(!collided) {
                                            if(x > 0) {
                                                rb.velocity.x += 1;
                                           } else if(x < 0) {
                                            rb.velocity.x -= 1;
                                           }
                                           if(y > 0) {
                                            rb.velocity.y += 1;
                                           } else {
                                            rb.velocity.y -= 1;
                                           }
    
                                           auto mag = std::sqrt(rb.velocity.x * rb.velocity.x + rb.velocity.y  * rb.velocity.y );
    
                                           if (mag != 0.0)
                                           {
                                            rb.velocity.x = (rb.velocity.x / mag);
                                            rb.velocity.y = (rb.velocity.y / mag);
                                           }
                                        rb.velocity.x *= PLAYER_SPEED;
                                        rb.velocity.y *= PLAYER_SPEED;
                                        }
                                      
                                    } });
        };
        void system_updateables_input()
        {
            system_updateables_input_player_keys();
            system_updateables_input_player_mouse();
        };
        void system_updateables_position(float dt)
        {
            auto updateables = world.view<SquareComponent, RigidBodyComponent>();
            updateables.for_each([&dt](SquareComponent &s, RigidBodyComponent &t)
                                 {
                                    float oldxvel = t.velocity.x;
                                    float oldyvel = t.velocity.y;
                                    t.velocity.x = 0;
                                    t.velocity.y = 0;
                t.transform.translation.x += (oldxvel * dt);
                t.transform.translation.y += (oldyvel * dt);
                s.ang = t.transform.rotation.x;
                s.rect.x = t.transform.translation.x;
                s.rect.y = t.transform.translation.y; });
        }

        void system_updateables_collider(float dt)
        {
            auto &prb = world.get<RigidBodyComponent>(player);
            auto collideables = world.view<RigidBodyComponent>();
            collideables.for_each([this, &prb, &dt](u64 id, RigidBodyComponent &rb)
                                  {
                                    if(id != this->player) {
                                        auto r = rb.collision_radius;
                                        float xsquared = (rb.transform.translation.x - prb.transform.translation.x) * (rb.transform.translation.x - prb.transform.translation.x);
                                        float ysquared = (rb.transform.translation.y - prb.transform.translation.y) * (rb.transform.translation.y - prb.transform.translation.y);
                                        float dist = std::sqrt(xsquared + ysquared);
                                        bool collided = dist < prb.collision_radius + r;
                                        if(collided) {
                                            prb.transform.translation.x += (-prb.velocity.x * dt);
                                            prb.transform.translation.y += (-prb.velocity.y * dt);
                                            rb.transform.translation.x += (-rb.velocity.x * dt);
                                            rb.transform.translation.y += (-rb.velocity.y * dt);
                                            world.get<HealthComponent>(player).hit_points--;
                                        }
                                    } });
        };

        void system_updateables(float dt)
        {
            system_updateables_input();
            system_updateables_position(dt);
            system_updateables_collider(dt);
            system_updateables_camera(dt);
            system_updateables_health();
            system_updateables_aggro();
        };

        void system_drawables_shapes()
        {
            auto drawable_entities = world.view<SquareComponent, CircleComponent>();
            drawable_entities.for_each([this](SquareComponent &sc, CircleComponent &cc)
                                       {
                                   // DrawRectangle(c.x, c.y, c.width, c.height, c.color);
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
            drawable_entities.for_each([this](const HealthComponent &hc)
                                       {
                                        DrawRectangleLines(hc.health_bar.x, hc.health_bar.y, hc.health_bar.width + 1, hc.health_bar.height + 1, BEIGE);
                                        DrawText(TextFormat("%d", hc.hit_points), hc.health_bar.x + hc.health_bar.width, hc.health_bar.y, 12, WHITE);
                                        DrawRectangleRec(hc.health_bar, get_health_color(hc.hit_points)); });
        };

        void system_drawables()
        {
            system_drawables_shapes();
            system_drawables_health();
        };

        void update_app_listener(int w, int h) override
        {
            screen_width = w;
            screen_height = h;
        };

    public:
        game() : player(spawn_player(100, 100, 500, 500, RED, {253, 76, 167, 47})), test(spawn_test(100, 100, 200, 200, BLUE, {253, 76, 167, 47})) {

                 };

        void update(float dt)
        {
            system_updateables(dt);
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
            float dt = GetFrameTime();
            update(dt);
            render();
        };
        ~game() = default;
    };
};
