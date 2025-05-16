#pragma once
#include <iostream>
#include "raylib.h"
#include "config.hpp"
#include "raymath.h"

#include "systems/updateables/system_camera.hpp"
#include "systems/updateables/system_physics.hpp"
#include "systems/updateables/system_input.hpp"
#include "systems/updateables/system_health.hpp"
#include "systems/updateables/system_ai.hpp"
#include "systems/drawables/system_drawables.hpp"
#include "state_manager.hpp"

#include "algorithm"
#include <string>
#include <cmath>
#include "rlgl.h"

namespace bden::state
{
    using namespace components;
    using namespace config;
    using namespace config::game_config;

    // TODO create glow effect for entites

    class state_game final : public bden::fsm::State<bden::fsm::states::APP_STATES>
    {

        using WorldType = snek::world<game_config::game_configuration_policy>;
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
            world.bind<AggroComponent>(test, w * 3, false);
            world.bind<WeaponComponent>(test, w * 1.5f, 5.f, 2.f, RED);
            auto c = world.bind<CircleComponent>(test, square.rect.width, glow_color);
            world.bind<RigidBodyComponent>(test, test_transform, Vector2(0, 0), c.radius);
            return test;
        };

    public:
        state_game(bden::fsm::StateManager<bden::fsm::states::APP_STATES> *ctx, RenderTexture2D &render_target) : State(ctx), player(spawn_player(100, 100, 500, 500, RED, {253, 76, 167, 47})),
                                                                                                                  test(spawn_test(100, 100, 200, 200, BLUE, {253, 76, 167, 47})),
                                                                                                                  camera_system(world),
                                                                                                                  physics_system(world),
                                                                                                                  input_system(world),
                                                                                                                  health_system(world, to_delete),
                                                                                                                  ai_system(world), render_system(world) {

                                                                                                                  };
        void on_update(float dt) override
        {
            if (!world.contains(player))
                return;

            physics_system.update(dt);
            camera_system.update(dt, player);
            input_system.update(player, camera_system);
            health_system.update(dt, player);
            ai_system.update(dt, player);
            system_updateables_delete_entities();
        };

        void on_render() override
        {
            float scale = std::min((float)GetScreenWidth() / app_config::VIRTUAL_WIDTH, (float)GetScreenHeight() / app_config::VIRTUAL_HEIGHT);
            BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(camera_system.get_camera());
            render_system.render();
            EndMode2D();
            EndDrawing();
        };

        void on_exit() override {};

        ~state_game() = default;
    };
};
