#pragma once
#include <iostream>
#include "raylib.h"
#include "internal.hpp"
#include "raymath.h"
#include "app_observer.hpp"
#include "systems/updateables/system_camera.hpp"
#include "systems/updateables/system_physics.hpp"
#include "systems/updateables/system_input.hpp"
#include "systems/updateables/system_health.hpp"
#include "systems/updateables/system_ai.hpp"
#include "systems/drawables/system_drawables.hpp"
#include "systems/drawables/system_ui.hpp"

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
        systems::HealthManager<WorldType::world_policy> health_system;
        systems::AiManager<WorldType::world_policy> ai_system;
        systems::DrawableManager<WorldType::world_policy> render_system;
        systems::UIManager<WorldType::world_policy> ui_system;

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

        void system_drawables()
        {
            render_system.render(player);
            ui_system.render();
        };

    public:
        game() : player(spawn_player(100, 100, 500, 500, RED, {253, 76, 167, 47})),
                 test(spawn_test(100, 100, 200, 200, BLUE, {253, 76, 167, 47})),
                 camera_system(world),
                 physics_system(world),
                 input_system(world),
                 health_system(world, to_delete),
                 ai_system(world), render_system(world), ui_system(world) {

                 };

        void update(float dt)
        {
            if (!world.contains(player))
                return;

            physics_system.update(dt, player);
            camera_system.update(dt, player);
            // TO DO separate player speed and camera system from input system
            input_system.update(player, camera_system, PLAYER_SPEED);
            health_system.update(dt, player);
            ai_system.update(dt, player, PLAYER_SPEED);
            system_updateables_delete_entities();
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
