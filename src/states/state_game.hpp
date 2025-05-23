#pragma once
#include <iostream>
#include "raylib.h"
#include "../config/player_config.hpp"
#include "../config/game_config.hpp"
#include "../config/weapon_config.hpp"
#include "../config/app_config.hpp"
#include "../config/enemy_config.hpp"

#include "raymath.h"

#include "../state_manager.hpp"
#include "../systems/drawables/system_drawables.hpp"
#include "../systems/drawables/system_ui.hpp"
#include "../systems/updateables/system_ai.hpp"
#include "../systems/updateables/system_camera.hpp"
#include "../systems/updateables/system_health.hpp"
#include "../systems/updateables/system_input.hpp"
#include "../systems/updateables/system_physics.hpp"
#include "../systems/updateables/system_weapons.hpp"

#include "algorithm"
#include <string>
#include <cmath>
#include "rlgl.h"

namespace bden::state
{
    using namespace components;
    using namespace config;

    // TODO create entity contracts, player must have component_set x,y,z... , enemy must have component_set y, z... etc.

    class state_game final : public bden::fsm::AppStateType
    {

        using WorldType = snek::world<GameConfig::game_configuration_policy>;
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
        systems::WeaponManager<WorldType::world_policy> weapon_system;

        void system_updateables_delete_entities()
        {
            for (auto &e : to_delete)
            {
                world.kill(e);
            }
            to_delete.clear();
        }

        WorldType::entity_type spawn_player(float w, float h, float x, float y)
        {
            using namespace config;
            auto p = world.spawn(GameConfig::TagEnum::TAG_PLAYER);
            SquareComponent square{w, h, x, y, PlayerConfig::PLAYER_COLOR, 0.0};
            world.bind<SquareComponent>(p, square);
            Transform player_transform{};
            player_transform.translation.x = x;
            player_transform.translation.y = y;

            const auto c = world.bind<CircleComponent>(p, square.rect.width, PlayerConfig::PLAYER_GLOW);
            Vector2 health_bar_pos(player_transform.translation.x, player_transform.translation.y - c.radius);
            Vector2 health_bar_size(w, 15.f);
            Rectangle health_bar_rect(health_bar_pos.x, health_bar_pos.y, health_bar_size.x, health_bar_size.y);
            world.bind<HealthComponent>(p, 100, health_bar_rect);
            world.bind<RigidBodyComponent>(p, player_transform, Vector2(0, 0), c.radius, PlayerConfig::PLAYER_SPEED);

            Vector2 weapon_ray{x, y};
            world.bind<WeaponComponent>(p, w * WeaponConfig::spear::radius, WeaponConfig::spear::damage, WeaponConfig::spear::speed, WeaponConfig::spear::color, weapon_ray);
            return p;
        };

        WorldType::entity_type spawn_test(float w, float h, float x, float y)
        {
            auto test = world.spawn(GameConfig::TagEnum::TAG_ENEMIES);
            using namespace config;

            SquareComponent square{w, h, x, y, EnemyConfig::ENEMY_COLOR, 0.0};
            world.bind<SquareComponent>(test, square);
            Transform test_transform{};

            test_transform.translation.x = x;
            test_transform.translation.y = y;
            Vector2 weapon_ray{x, y};
            world.bind<AggroComponent>(test, w * 3, false);
            world.bind<WeaponComponent>(test, w * WeaponConfig::spear::radius, WeaponConfig::spear::damage, WeaponConfig::spear::speed, WeaponConfig::spear::color, weapon_ray);
            auto c = world.bind<CircleComponent>(test, square.rect.width, EnemyConfig::ENEMY_GLOW);
            world.bind<RigidBodyComponent>(test, test_transform, Vector2(0, 0), c.radius, EnemyConfig::ENEMY_SPEED);

            Vector2 health_bar_pos(test_transform.translation.x, test_transform.translation.y - c.radius);
            Vector2 health_bar_size(w, 15.f);
            Rectangle health_bar_rect(health_bar_pos.x, health_bar_pos.y, health_bar_size.x, health_bar_size.y);
            world.bind<HealthComponent>(test, 100, health_bar_rect);

            return test;
        };

    public:
        state_game(bden::fsm::StateManager<bden::fsm::states::APP_STATES> *ctx, RenderTexture2D &render_target) : State(ctx),
                                                                                                                  camera_system(world),
                                                                                                                  physics_system(world),
                                                                                                                  input_system(world),
                                                                                                                  health_system(world, to_delete),
                                                                                                                  ai_system(world), render_system(world), weapon_system(world) {
                                                                                                                  };

        bool on_init() override
        {
            this->player = (spawn_player(100, 100, 500, 500));
            this->test = (spawn_test(100, 100, 200, 200));
            return true;
        };

        void on_update(float dt) override
        {
            if (!world.contains(player))
                return;

            if (WindowShouldClose())
            {
                this->get_context()->set_state(bden::fsm::AppStateManagerType::states_type::STATE_QUIT);
            }

            physics_system.update(dt);
            camera_system.update(dt, player);
            input_system.update(player, camera_system);

            health_system.update(dt, player);

            ai_system.update(dt, player);
            weapon_system.update(dt, player);

            system_updateables_delete_entities();
        };

        void on_render() override
        {
            float scale = std::min((float)GetScreenWidth() / AppConfig::VIRTUAL_WIDTH, (float)GetScreenHeight() / AppConfig::VIRTUAL_HEIGHT);
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